#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Assert.hpp"
#include "Camera.hpp"
#include "EBO.hpp"
#include "Globals.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Shader.hpp"
#include "ShaderCollection.hpp"
#include "ShaderUniforms.hpp"
#include "Stopwatch.hpp"
#include "VBO.hpp"
#include "Window.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstddef>
#include <cstdint>
#include <entt.hpp>
#include <glad/gl.h>
#include <memory>

// Make sure that all of the datatypes are of correct size
// so that they work correctly with OpenGL
static_assert(sizeof(f64) == sizeof(GLdouble));
static_assert(sizeof(f32) == sizeof(GLfloat));
static_assert(sizeof(int) == sizeof(GLint));
#ifdef BIRB_PLATFORM_LINUX
static_assert(sizeof(long int) == sizeof(GLint64));
#else
#pragma message "WARNING: long int != GLint64 with mingw"
#endif
static_assert(sizeof(short) == sizeof(GLshort));
static_assert(sizeof(std::byte) == sizeof(GLbyte));
static_assert(sizeof(unsigned int) == sizeof(GLuint));
#ifdef BIRB_PLATFORM_LINUX
static_assert(sizeof(unsigned long int) == sizeof(GLuint64));
#else
#pragma message "WARNING: unsigned long int != GLint64 with mingw"
#endif
static_assert(sizeof(unsigned short) == sizeof(GLushort));
static_assert(sizeof(size_t) == sizeof(GLsizeiptr));
static_assert(sizeof(intptr_t) == sizeof(GLintptr));

static_assert(sizeof(i8) == sizeof(GLchar));
static_assert(sizeof(i16) == sizeof(GLshort));
static_assert(sizeof(i32) == sizeof(GLint));
static_assert(sizeof(i64) == sizeof(GLint64));
static_assert(sizeof(u8) == sizeof(GLchar));
static_assert(sizeof(u16) == sizeof(GLushort));
static_assert(sizeof(u32) == sizeof(GLuint));
static_assert(sizeof(u64) == sizeof(GLuint64));

static_assert(static_cast<GLenum>(birb::renderer::gl_primitive::triangles) == GL_TRIANGLES);
static_assert(static_cast<GLenum>(birb::renderer::gl_primitive::lines) == GL_LINES);
static_assert(static_cast<GLenum>(birb::renderer::gl_type::unsigned_int) == GL_UNSIGNED_INT);

namespace birb
{
	renderer::renderer()
	 :line_shader("line"),
	 view_matrix_ubo(shader_uniforms::block::view_matrices),
	 debug_shader_ref("color"),
	 texture_shader_ref("texture"),
	 post_processing_shader_ref("post_process")
	{
		event_bus::register_event_id(event::toggle_wireframe_rendering_mode, this);
		event_bus::register_event_id(event::toggle_debug_view, this);
		event_bus::register_event_id(event::reload_models, this);

		// Enable blending by default
		opt_blend(true);

		// Enable gamma correction by default
		opt_gamma_correction(true);

		// Enable backface culling by default
		set_backface_culling(true);

		// Initialize the opengl buffers for sprite rendering
		sprite_vao.bind();

		sprite_vbo = std::make_unique<vbo>(quad_vertices);
		sprite_ebo = std::make_unique<ebo>(quad_indices);

		constexpr u32 vertex_pos_layout = 0;
		constexpr u32 texture_coord_layout = 1;

		constexpr u32 vertex_component_count = 3;
		constexpr u32 texture_coord_component_count = 2;

		constexpr u32 vertex_offset = 0;
		constexpr u32 texture_coord_offset = vertex_component_count;

		constexpr u32 stride = 5;

		sprite_vao.link_vbo(*sprite_vbo,
				vertex_pos_layout,
				vertex_component_count,
				stride,
				vertex_offset);

		sprite_vao.link_vbo(*sprite_vbo,
				texture_coord_layout,
				texture_coord_component_count,
				stride,
				texture_coord_offset);

		sprite_vao.unbind();
		sprite_vbo->unbind();
		sprite_ebo->unbind();


		// Initialize things for text rendering
		text_vao.bind();

		text_vbo.bind();
		text_vbo.set_data(NULL, sizeof(f32) * 6 * 4, false); // 6 verts, 4 floats per vert
		text_vbo.enable_vertex_attrib_array(0);
		text_vbo.set_vertex_attrib_ptr(0, 0, sizeof(f32), 4);

		text_vao.unbind();
		text_vbo.unbind();


		// Initialize things for the collider debug cube
		collider_debug_color = 0xC34043;

		collider_debug_vao.bind();

		collider_debug_vbo = std::make_unique<vbo>(cube_vertices);
		collider_debug_vao.link_vbo(*collider_debug_vbo, 0, 3, 3, 0);
		collider_debug_vao.unbind();
		collider_debug_vbo->unbind();


		// Initialize things for post-processing
		post_processing_fbo = std::make_unique<fbo>(vec2<i32>(1920, 720), color_format::RGB);
		post_processing_vbo = std::make_unique<vbo>(fullscreen_quad_vertices);

		post_processing_vao.bind();
		post_processing_vao.link_ebo(*sprite_ebo);
		post_processing_vao.link_vbo(*post_processing_vbo, 0, 3, 5, 0);
		post_processing_vao.link_vbo(*post_processing_vbo, 1, 2, 5, 3);
		post_processing_vao.unbind();
		sprite_ebo->unbind();

		// Precompile shaders in release builds
#ifdef NDEBUG
		shader_collection::precompile_basic_shaders();
#else
		birb::log_warn("Shader precompiling is disabled in debug builds");
#endif
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
		event_bus::unregister_event_id(event::toggle_wireframe_rendering_mode, this);
		event_bus::unregister_event_id(event::toggle_debug_view, this);
		event_bus::unregister_event_id(event::reload_models, this);
	}

	void renderer::process_event(u16 event_id, const event_data& data)
	{
		switch (event_id)
		{
			case event::toggle_wireframe_rendering_mode:
				toggle_wireframe();
				break;

			case event::toggle_debug_view:
				toggle_debug_view();
				break;

			case event::reload_models:
				current_scene->reload_models();
				break;
		}
	}

	void renderer::set_scene(scene& scene)
	{
		ensure(scene::scene_count() > 0);
		current_scene = &scene;
	}

	void renderer::set_window(birb::window& window)
	{
		this->window = &window;
	}

	void renderer::draw_entities(const camera& camera, vec2<i32> window_size)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(current_scene != nullptr);
		ensure(scene::scene_count() > 0);
		ensure(!g_buffers_flipped, "Tried to draw entities after the buffers were already flipped");

		// Bind the post-processing frame buffer and update its dimensions if needed
		if (post_processing_enabled)
		{
			post_processing_fbo->bind();
			if (window->size() != old_window_dimensions)
			{
				post_processing_fbo->reload_frame_buffer_texture(window->size());
				old_window_dimensions = window->size();
			}

			post_processing_fbo->clear();
		}

		entt::registry& entity_registry = current_scene->registry;

		glm::mat4 view_matrix = camera.view_matrix();

		// Update the view matrix
		view_matrix_ubo.update_data(glm::value_ptr(camera.view_matrix()), sizeof(glm::mat4), 0);

		// Reset statistics
		render_stats.reset_counters();

		birb::stopwatch render_stopwatch;
		draw_2d_entities();
		render_stats.draw_2d_duration = render_stopwatch.stop(true);

		render_stopwatch.reset();
		draw_3d_entities();
		render_stats.draw_3d_duration = render_stopwatch.stop(true);

		render_stopwatch.reset();
		draw_screenspace_entities();
		render_stats.draw_screenspace_duration = render_stopwatch.stop(true);


		/////////////////////
		// Post-processing //
		/////////////////////

		// Unbind the framebuffer object and draw the scene into a texture
		if (post_processing_enabled)
		{
			post_processing_fbo->unbind();

			std::shared_ptr<shader> post_shader = shader_collection::get_shader(post_processing_shader_ref);
			post_shader->activate();
			post_processing_fbo->bind_frame_buffer();
			opt_depth_test(false);
			draw_elements(post_processing_vao, quad_indices.size());
			opt_depth_test(true);
			post_processing_fbo->unbind_frame_buffer();
		}

		frame_id_counter++;

		/*****************************************************************************/
		/* Nothing will be rendered after this point if debug drawing is not enabled */
		/*****************************************************************************/

#ifndef BIRB_RELEASE

		// If debug drawing is not enabled, we can stop drawing stuff here
		if (!debug_view_enabled)
			return;

		///////////////////
		// Debug drawing //
		///////////////////

		// Draw the debug view
		debug.draw();

#endif
	}

	void renderer::draw_elements(size_t index_count, gl_primitive primitive)
	{
		ensure(index_count > 0, "Unncessary call to draw_elements()");
		glDrawElements(static_cast<GLenum>(primitive), index_count, GL_UNSIGNED_INT, 0);
		++render_stats.draw_elements_calls;
	}

	void renderer::draw_elements(vao& vao, size_t index_count, gl_primitive primitive)
	{
		ensure(vao.id != 0);
		ensure(vao.contains_ebo());
		ensure(index_count > 0, "Unncessary call to draw_elements()");

		vao.bind();
		glDrawElements(static_cast<GLenum>(primitive), index_count, GL_UNSIGNED_INT, 0);
		++render_stats.draw_elements_vao_calls;
	}

	void renderer::draw_elements_instanced(const u32 index_count, const u32 instance_count)
	{
		ensure(index_count > 0);
		ensure(instance_count > 0);

		glDrawElementsInstanced(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0, instance_count);
		++render_stats.draw_elements_instanced;
	}

	void renderer::draw_elements_instanced(const std::vector<u32>& indices, const u32 instance_count)
	{
		ensure(!indices.empty());
		ensure(instance_count > 0);

		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instance_count);
		++render_stats.draw_elements_instanced;
	}

	void renderer::draw_elements_instanced(vao& vao, const std::vector<u32>& indices, const u32 instance_count)
	{
		ensure(vao.id != 0);
		ensure(!indices.empty());
		ensure(instance_count > 0);

		vao.bind();
		glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instance_count);
		++render_stats.draw_elements_instanced;
	}

	void renderer::draw_arrays(size_t vert_count, gl_primitive primitive)
	{
		ensure(vert_count > 0, "Unncessary call to draw_arrays()");
		glDrawArrays(static_cast<GLenum>(primitive), 0, vert_count);
		++render_stats.draw_arrays_calls;
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count, gl_primitive primitive)
	{
		ensure(vao.id != 0);
		ensure(vert_count > 0, "Unncessary call to draw_arrays()");

#ifndef NDEBUG
		// TODO: This assert is a bit questionable and needs more attention
		// ensure(vao.d_total_vbo_vert_count == vert_count, "Something has gone wrong with VBO linking");
#endif

		vao.bind();
		glDrawArrays(static_cast<GLenum>(primitive), 0, vert_count);
		++render_stats.draw_arrays_vao_calls;
	}

	void renderer::toggle_wireframe()
	{
		wireframe_mode = !wireframe_mode;

		if (!wireframe_mode)
		{
			birb::log("Toggling wireframe mode off");
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			birb::log("Toggling wireframe mode on");
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

	}

	bool renderer::is_wireframe_enabled()
	{
		return wireframe_mode;
	}

	void renderer::set_backface_culling(bool state)
	{
		if (state)
			glEnable(GL_CULL_FACE);
		else
			glDisable(GL_CULL_FACE);

		backface_culling_enabled = state;
	}

	bool renderer::is_backface_culling_enabled()
	{
		return backface_culling_enabled;
	}

	void renderer::toggle_debug_view()
	{
#ifndef BIRB_RELEASE
		debug_view_enabled = !debug_view_enabled;

		if (!debug_view_enabled)
			birb::log("Toggling debug view off");
		else
			birb::log("Toggling debug view on");
#endif
	}

   	renderer_stats renderer::rendering_statistics() const
	{
		return render_stats;
	}

	void renderer::opt_shadows(const bool enabled)
	{
		shadows_enabled = enabled;
	}

	void renderer::opt_blend(const bool enabled) const
	{
		ensure(g_opengl_initialized);

		if (enabled)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	void renderer::opt_depth_test(const bool enabled) const
	{
		ensure(g_opengl_initialized);

		if (enabled)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);
	}

	void renderer::opt_gamma_correction(const bool enabled) const
	{
		ensure(g_opengl_initialized);

		if (enabled)
			glEnable(GL_FRAMEBUFFER_SRGB);
		else
			glDisable(GL_FRAMEBUFFER_SRGB);
	}

	void renderer::opt_post_process(const bool enabled)
	{
		ensure(window != nullptr, "The window needs to be set before post-processing can be enabled");
		post_processing_enabled = enabled;
	}

	bool renderer::is_post_processing_enabled() const
	{
		return post_processing_enabled;
	}
}

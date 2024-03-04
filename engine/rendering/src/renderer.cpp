#include "Logger.hpp"
#include "Model.hpp"
#include "Renderer.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"
#include "Transform.hpp"
#include "glm/fwd.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <entt.hpp>
#include <glad/gl.h>

// Make sure that all of the datatypes are of correct size
// so that they work correctly with OpenGL
static_assert(sizeof(double) == sizeof(GLdouble));
static_assert(sizeof(float) == sizeof(GLfloat));
static_assert(sizeof(int) == sizeof(GLint));
static_assert(sizeof(long int) == sizeof(GLint64));
static_assert(sizeof(short) == sizeof(GLshort));
static_assert(sizeof(std::byte) == sizeof(GLbyte));
static_assert(sizeof(unsigned int) == sizeof(GLuint));
static_assert(sizeof(unsigned long int) == sizeof(GLuint64));
static_assert(sizeof(unsigned short) == sizeof(GLushort));
static_assert(sizeof(size_t) == sizeof(GLsizeiptr));
static_assert(sizeof(intptr_t) == sizeof(GLintptr));

namespace birb
{
	renderer::renderer()
	{
		event_bus::register_event_id(1, this); // Toggle wireframe mode
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
		event_bus::unregister_event_id(1, this);
	}

	void renderer::process_event(unsigned short event_id, const event_data& data)
	{
		switch (event_id)
		{
			case 1:
				toggle_wireframe();
				break;
		}
	}

	void renderer::set_scene(scene& scene)
	{
		current_scene = &scene;
	}

	void renderer::draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(current_scene != nullptr);

		entt::registry& entity_registry = current_scene->get_registry();

		// Render all models
		rendered_entities = 0;
		auto view = entity_registry.view<birb::model, birb::shader, birb::component::transform>();
		for (auto ent : view)
		{
			// Get the shader we'll be using for drawing the meshes of the model
			shader& shader = view.get<birb::shader>(ent);

			const birb::component::transform& transform = view.get<birb::component::transform>(ent);

			// Calculate the model transform
			glm::mat4 model_matrix = glm::mat4(1.0f);
			model_matrix = glm::translate(model_matrix, transform.position.to_glm_vec());

			glm::vec3 euler_angles({glm::radians(transform.rotation.x), glm::radians(transform.rotation.y), glm::radians(transform.rotation.z)});
			glm::quat quaternion(euler_angles);
			glm::mat4 rotation_matrix(quaternion);
			model_matrix = model_matrix * rotation_matrix;

			shader.set_mat4("model", model_matrix);
			shader.set_mat4("view", view_matrix);
			shader.set_mat4("projection", projection_matrix);

			// Draw the model
			view.get<birb::model>(ent).draw(shader);
			++rendered_entities;
		}
	}

	void renderer::draw_elements(vao& vao, size_t index_count)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(vao.id != 0);
		assert(index_count > 0 && "Unncessary call to draw_elements()");

		vao.bind();
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count)
	{
		PROFILER_SCOPE_RENDER_FN()

		assert(vao.id != 0);
		assert(vert_count > 0 && "Unncessary call to draw_arrays()");

		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vert_count);
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

	unsigned int renderer::rendered_entities_count() const
	{
		return rendered_entities;
	}
}

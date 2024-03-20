#pragma once

#include "EventBus.hpp"
#include "Scene.hpp"
#include "VAO.hpp"

#include <entt.hpp>
#include <glm/fwd.hpp>

namespace birb
{
	class renderer : public event_obj
	{
	public:
		renderer();
		renderer(const renderer&) = delete;
		~renderer();

		void process_event(unsigned short event_id, const event_data& data) override;

		void set_scene(scene& scene);

		void draw_entities(const glm::mat4& view_matrix, const glm::mat4& projection_matrix);
		void draw_elements(vao& vao, size_t index_count);
		void draw_arrays(vao& vao, size_t vert_count);

		static void toggle_wireframe();
		static bool is_wireframe_enabled();

		static void set_backface_culling(bool state);
		static bool is_backface_culling_enabled();

		/**
		 * @return The amount of entities rendered during last call to draw_entities()
		 */
		unsigned int rendered_entities_count() const;

		/**
		 * @return The amount of vertices rendered during last call to draw_entities()
		 */
		unsigned int rendered_vertex_count() const;

	private:
		scene* current_scene = nullptr;
		unsigned int shader_program;
		unsigned int VAO, VBO;
		static inline bool wireframe_mode = false;
		static inline bool backface_culling_enabled = false;

		// Variables for rendering statistics
		unsigned int rendered_entities = 0;
		unsigned int rendered_vertices = 0;
	};
}

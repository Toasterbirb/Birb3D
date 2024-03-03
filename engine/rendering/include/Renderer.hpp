#pragma once

#include "EventBus.hpp"
#include "Scene.hpp"
#include "VAO.hpp"

namespace birb
{
	class renderer : public event_obj
	{
	public:
		renderer();
		~renderer();

		void process_event(unsigned short event_id, const event_data& data);

		void set_scene(scene& scene);

		void draw_entities();
		void draw_elements(vao& vao, size_t index_count);
		void draw_arrays(vao& vao, size_t vert_count);
		static void toggle_wireframe();
		static bool is_wireframe_enabled();

		/**
		 * @return The amount of entities rendered during last call to draw_entities()
		 */
		unsigned int rendered_entities_count() const;

	private:
		scene* current_scene = nullptr;
		unsigned int shader_program;
		unsigned int VAO, VBO;
		static inline bool wireframe_mode = false;

		// Variables for rendering statistics
		unsigned int rendered_entities = 0;
	};
}

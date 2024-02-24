#pragma once

#include "EventBus.hpp"
#include "VAO.hpp"

namespace birb
{
	class renderer : public event_obj
	{
	public:
		renderer();
		~renderer();

		void process_event(unsigned short event_id, const event_data& data);

		void draw_elements(vao& vao, size_t index_count);
		void draw_arrays(vao& vao, size_t vert_count);
		static void toggle_wireframe();

	private:
		unsigned int shader_program;
		unsigned int VAO, VBO;
		static inline bool wireframe_mode = false;
	};
}

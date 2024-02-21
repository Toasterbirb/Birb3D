#pragma once

#include "EventBus.hpp"

#include <vector>

namespace birb
{
	class renderer : public event_obj
	{
	public:
		renderer();
		~renderer();

		void process_event(unsigned short event_id, const event_data& data);

		void draw_verts(std::vector<float> verts);
		static void toggle_wireframe();

	private:
		unsigned int shader_program;
		unsigned int VAO, VBO;
		static inline bool wireframe_mode = false;
	};
}

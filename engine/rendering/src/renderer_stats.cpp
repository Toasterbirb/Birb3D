#include "RendererStats.hpp"

namespace birb
{
	u32 renderer_stats::total_entities() const
	{
		return entities_2d + entities_3d + entities_screenspace;
	}

	u32 renderer_stats::total_vertices() const
	{
		return vertices_2d + vertices_3d + vertices_screenspace;
	}

	f64 renderer_stats::total_duration() const
	{
		return draw_2d_duration + draw_3d_duration + draw_screenspace_duration;
	}

	u32 renderer_stats::total_draw_calls() const
	{
		return draw_arrays_calls + draw_elements_calls;
	}

	void renderer_stats::reset_counters()
	{
		entities_2d = 0;
		entities_3d = 0;
		entities_screenspace = 0;

		vertices_2d = 0;
		vertices_3d = 0;
		vertices_screenspace = 0;

		draw_arrays_calls = 0;
		draw_elements_calls = 0;
		draw_arrays_vao_calls = 0;
		draw_elements_vao_calls = 0;
		draw_arrays_instanced = 0;
	}
}

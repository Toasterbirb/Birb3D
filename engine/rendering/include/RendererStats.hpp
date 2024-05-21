#pragma once

namespace birb
{
	struct renderer_stats
	{
		u32 entities_2d = 0;
		u32 entities_3d = 0;
		u32 entities_screenspace = 0;

		u32 vertices_2d = 0;
		u32 vertices_3d = 0;
		u32 vertices_screenspace = 0;

		f64 draw_2d_duration = 0.0f;
		f64 draw_3d_duration = 0.0f;
		f64 draw_screenspace_duration = 0.0f;

		u32 draw_arrays_calls = 0;
		u32 draw_elements_calls = 0;
		u32 draw_arrays_vao_calls = 0;
		u32 draw_elements_vao_calls = 0;
		u32 draw_arrays_instanced = 0;

		u32 total_entities() const;
		u32 total_vertices() const;
		f64 total_duration() const;
		u32 total_draw_calls() const;
		void reset_counters();
	};
}

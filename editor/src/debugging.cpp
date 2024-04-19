#include "Debugging.hpp"
#include "Profiling.hpp"
#include "Shader.hpp"

#include <imgui.h>

namespace editor
{
	void debugging::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Debugging");
		{
			ImGui::SeparatorText("Shaders");

			// Cache
			size_t shader_cache_size = birb::shader::shader_cache_size();
#ifdef BIRB_PLATFORM_LINUX
			ImGui::Text("Shaders cached: %lu", shader_cache_size);
			ImGui::Text("Shader cache hits: %lu", birb::shader::shader_cache_hits());
#else
			ImGui::Text("Shaders cached: %llu", shader_cache_size);
			ImGui::Text("Shader cache hits: %llu", birb::shader::shader_cache_hits());
#endif
			if (shader_cache_size > 0 && ImGui::Button("Clear cache"))
				birb::shader::clear_shader_cache();
		}
		ImGui::End();
	}
}

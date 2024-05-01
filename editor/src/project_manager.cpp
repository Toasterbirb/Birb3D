#include "Profiling.hpp"
#include "Project.hpp"
#include "ProjectManager.hpp"
#include "Viewport.hpp"

#include <GLFW/glfw3.h>
#include <cmath>

namespace editor
{
	project_manager::project_manager(birb::project& project, editor::viewport& viewport)
	:project(project), viewport(viewport)
	{}

	void project_manager::draw()
	{
		PROFILER_SCOPE_RENDER_FN()

		ImGui::Begin("Project");
		{
			f64 now = glfwGetTime();
			f64 duration = now - last_save;

			std::string time_text = "";
			if (duration > 60)
			{
				int seconds = static_cast<int>(duration) % 60;
				time_text = std::to_string(static_cast<int>(std::round(duration / 60))) + "min " + std::to_string(seconds) + "s";
			}
			else
				time_text = std::to_string(static_cast<int>(std::round(duration)));

			ImGui::Text("Time since last save: %s", time_text.c_str());

			if (ImGui::Button("Save"))
			{
				project.save(viewport.camera);
				last_save = glfwGetTime();
			}

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("Loaded version: %d", project.loaded_version);
			ImGui::Text("Editor version: %d", project.version);
		}
		ImGui::End();
	}
}

#include "Assert.hpp"
#include "Logger.hpp"
#include "MainMenuBar.hpp"
#include "Project.hpp"
#include "Viewport.hpp"

namespace editor
{
	main_menu_bar::main_menu_bar(birb::project& project, editor::viewport& viewport)
	:project(project), viewport(viewport)
	{}

	void main_menu_bar::save_project()
	{
		project.save(viewport.camera);
	}

	void main_menu_bar::draw()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open project"))
				{
					constexpr u16 max_filename_len = 1024;
					char filename[max_filename_len];
					FILE* file = popen("zenity --file-selection", "r");
					ASSERT(fgets(filename, max_filename_len, file) != nullptr);
					birb::log("File name: ", filename);
				}

				if (ImGui::MenuItem("Save"))
					save_project();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}

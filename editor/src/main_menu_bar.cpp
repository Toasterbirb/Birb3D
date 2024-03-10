#include "MainMenuBar.hpp"

namespace editor
{
	main_menu_bar::main_menu_bar(birb::project& project, editor::viewport& viewport)
	:project(project), viewport(viewport)
	{}

	void main_menu_bar::save_project()
	{
		birb::log("Saving the project to " + project.path());
		project.save_camera_settings(viewport.camera);
		project.save();
	}

	void main_menu_bar::draw()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open project"))
				{
					constexpr unsigned int max_filename_len = 1024;
					char filename[max_filename_len];
					FILE* file = popen("zenity --file-selection", "r");
					fgets(filename, max_filename_len, file);
					birb::log("File name", filename);
				}

				if (ImGui::MenuItem("Save"))
					save_project();

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
	}
}

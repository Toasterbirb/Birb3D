#pragma once

#include "Components.hpp"
#include "EditorWindow.hpp"
#include "Scene.hpp"

namespace editor
{
	class inspector : public editor_window
	{
	public:
		explicit inspector(birb::scene& scene);

		void draw() override;

	private:
		birb::scene& scene;

		static inline const std::vector<std::pair<std::string, birb::component::transform>> component_types = {
			{ "Transform", birb::component::transform() }
		};

		static inline std::string component_type_str = "";
	};
}

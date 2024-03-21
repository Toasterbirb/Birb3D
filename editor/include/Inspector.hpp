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

		enum class component_type
		{
			transform,
			material,
		};

		static inline const std::vector<std::pair<std::string, component_type>> component_names = {
			{ "Transform", component_type::transform },
			{ "Material", component_type::material },
		};

		static inline std::string component_type_str = "";
	};
}

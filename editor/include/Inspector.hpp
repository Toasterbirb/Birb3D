#pragma once

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
	};
}

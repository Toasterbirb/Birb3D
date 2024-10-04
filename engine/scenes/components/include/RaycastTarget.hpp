#pragma once

#include "EditorComponent.hpp"
#include "Types.hpp"
#include "Vector.hpp"

namespace birb
{
	class raycast_target : public editor_component
	{
	public:
		vec3<f32> position;
		f32 radius;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

	private:
		static inline const std::string editor_header_name = "Raycast target";
	};
}

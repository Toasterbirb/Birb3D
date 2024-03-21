#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"

namespace birb
{
	namespace component
	{
		struct material : public editor_component
		{
			material();
			material(const color& diffuse, const color& specular, float shininess = 32);

			void draw_editor_ui() override;

			color diffuse, specular;
			float shininess;

			template<class Archive>
				void serialize(Archive& ar)
				{
					ar(diffuse, specular, shininess);
				}
		};
	}
}

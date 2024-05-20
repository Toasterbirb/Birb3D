#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"

namespace birb
{
	class material : public editor_component
	{
	public:
		material();
		material(const color& diffuse, const color& specular, f32 shininess = 32);

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		color diffuse, specular;
		f32 shininess;

		// 0 = color
		// 1 = texture
		u8 type;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(diffuse, specular, shininess);
		}

	private:
		static inline const std::string editor_header_name = "Material";
	};
}

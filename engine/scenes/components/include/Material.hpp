#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"

namespace birb
{
	namespace component
	{
		class material : public editor_component
		{
		public:
			material();
			material(const color& diffuse, const color& specular, float shininess = 32);

			void draw_editor_ui() override;
			std::string collapsing_header_name() const override;

			color diffuse, specular;
			float shininess;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(diffuse, specular, shininess);
			}

		private:
			static inline const std::string editor_header_name = "Material";
		};
	}
}

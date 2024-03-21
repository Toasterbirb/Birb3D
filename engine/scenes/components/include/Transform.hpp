#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"

#include <glm/glm.hpp>

namespace birb
{
	namespace component
	{
		class transform : public editor_component
		{
		public:
			transform();

			void draw_editor_ui() override;
			std::string collapsing_header_name() const override;

			vec3<float> position;
			vec3<float> rotation;
			vec3<float> local_scale;

			glm::mat4 model_matrix() const;

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(position, rotation, local_scale);
			}

		private:
			const std::string editor_header_name = "Transform";
		};
	}
}

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

			vec3<f32> position;
			vec3<f32> rotation;
			vec3<f32> local_scale;

			glm::mat4 model_matrix() const;

			/**
			 * @brief Lock the model matrix to the current value
			 *
			 * This can be used to improve performance with entities that
			 * don't move or change their rotation / local_scale etc.
			 */
			void lock();

			/**
			 * @brief Enable model_matrix updates
			 */
			void unlock();

			template<class Archive>
			void serialize(Archive& ar)
			{
				ar(position, rotation, local_scale);
			}

		private:
			static inline const std::string editor_header_name = "Transform";
			glm::mat4 cached_model_matrix;
			bool is_locked = false;
		};
	}
}

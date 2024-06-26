#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"

#include <glm/glm.hpp>

namespace birb
{
	class transform : public editor_component
	{
	public:
		transform();
		virtual ~transform() = default;

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		vec3<f32> position;
		vec3<f32> rotation;
		vec3<f32> local_scale;

		glm::mat4 model_matrix() const;

		/**
		 * @brief Move the transform position with the given delta values
		 */
		void translate(f32 x, f32 y, f32 z);

		/**
		 * @brief Rotate the transform with the given delta values
		 */
		void rotate(f32 x, f32 y, f32 z);

		/**
		 * @brief Multiply the scale of the transform with the given values
		 */
		void scale(f32 x, f32 y, f32 z);

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

		bool is_locked() const;

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(position, rotation, local_scale);
		}

	private:
		static inline const std::string editor_header_name = "Transform";
		glm::mat4 cached_model_matrix;
		bool _is_locked = false;
	};
}

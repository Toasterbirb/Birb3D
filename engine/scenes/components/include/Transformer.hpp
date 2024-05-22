#pragma once

#include <glm/glm.hpp>

namespace birb
{
	class transform;

	class transformer
	{
	public:
		transformer() = default;
		~transformer();
		transformer(const transformer&) = default;
		transformer(transformer&) = default;

		std::vector<transform> transforms;

		u32 model_matrix_instance_vbo();
		void bind_vbo() const;

		/**
		 * @brief Lock the transforms and cache the model matrices
		 */
		void lock();

		/**
		 * @brief Unlock all transforms
		 */
		void unlock();

		/**
		 * @brief Unlock and lock the transforms to re-calculate the model matrices
		 */
		void relock();

		/**
		 * @brief Update the model matrix for a given transform without relocking the transformer
		 */
		void update_transform(const size_t index);

		/**
		 * @brief Update the instance VBO
		 *
		 * Call this function if you have modified transforms with update_transform()
		 */
		void update_vbo_data();

		bool is_locked() const;

	private:
		std::vector<glm::mat4> cached_model_matrices;
		bool _is_locked = false;

		bool d_singular_transform_updated = false;

		u32 model_matrix_vbo = 0;

		std::vector<glm::mat4> model_matrices() const;
		void update_model_matrix_vbo();
		void free_the_vbo_buffer();
	};
}

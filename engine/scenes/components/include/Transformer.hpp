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

		bool is_locked() const;

	private:
		std::vector<glm::mat4> cached_model_matrices;
		bool _is_locked = false;

		u32 model_matrix_vbo = 0;

		std::vector<glm::mat4> model_matrices() const;
		void update_model_matrix_vbo();
		void free_the_vbo_buffer();
	};
}

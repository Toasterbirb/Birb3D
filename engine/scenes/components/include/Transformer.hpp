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

		std::vector<glm::mat4> model_matrices() const;
		u32 model_matrix_instance_vbo() const;
		void bind_vbo() const;

		/**
		 * @brief Lock the transforms and cache the model matrices
		 */
		void lock();

		/**
		 * @brief Unlock all transforms
		 */
		void unlock();

		bool is_locked() const;

	private:
		std::vector<glm::mat4> cached_model_matrices;
		bool _is_locked = false;

		u32 model_matrix_vbo = 0;

		u32 create_model_matrix_vbo() const;
	};
}

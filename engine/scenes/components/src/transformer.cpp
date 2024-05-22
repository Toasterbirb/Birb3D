#include "Assert.hpp"
#include "Transform.hpp"
#include "Transformer.hpp"
#include "Types.hpp"
#include <cstdlib>

namespace birb
{
	transformer::~transformer()
	{
		if (model_matrix_vbo != 0)
			glDeleteBuffers(1, &model_matrix_vbo);
	}

	u32 transformer::model_matrix_instance_vbo() const
	{
		if (_is_locked)
			return model_matrix_vbo;

		return create_model_matrix_vbo();
	}

	void transformer::bind_vbo() const
	{
		ensure(model_matrix_vbo != 0);
		glBindBuffer(GL_ARRAY_BUFFER, model_matrix_vbo);
	}

	void transformer::lock()
	{
		cached_model_matrices.clear();
		cached_model_matrices.reserve(transforms.size());

		for (transform t : transforms)
		{
			t.lock();
			cached_model_matrices.push_back(t.model_matrix());
		}

		model_matrix_vbo = model_matrix_instance_vbo();
		_is_locked = true;
	}

	void transformer::unlock()
	{
		// Unlock all of the child transforms
		for (transform t : transforms)
			t.unlock();

		_is_locked = false;
	}

	void transformer::relock()
	{
		unlock();
		lock();
	}

	bool transformer::is_locked() const
	{
		return _is_locked;
	}

	std::vector<glm::mat4> transformer::model_matrices() const
	{
		if (_is_locked)
			return cached_model_matrices;

		std::vector<glm::mat4> tmp_model_matrices(transforms.size());
		for (size_t i = 0; i < transforms.size(); ++i)
			tmp_model_matrices[i] = transforms[i].model_matrix();

		return tmp_model_matrices;
	}

	u32 transformer::create_model_matrix_vbo() const
	{
		ensure(model_matrix_vbo == 0, "Free the previous VBO before creating a new one");
		ensure(!transforms.empty(), "Can't create a VBO when there are no transforms");

		std::vector<glm::mat4> matrices = model_matrices();

		u32 vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);

		return vbo;
	}
}

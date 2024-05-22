#include "Assert.hpp"
#include "Profiling.hpp"
#include "Transform.hpp"
#include "Transformer.hpp"
#include "Types.hpp"

#include <cstdlib>

namespace birb
{
	transformer::~transformer()
	{
		if (model_matrix_vbo != 0)
			free_the_vbo_buffer();
	}

	u32 transformer::model_matrix_instance_vbo()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(_is_locked, "Transformer needs to be locked before it can be rendered");
		ensure(model_matrix_vbo != 0);

#ifndef NDEBUG
		ensure(!d_singular_transform_updated);
#endif

		return model_matrix_vbo;
	}

	void transformer::bind_vbo() const
	{
		ensure(model_matrix_vbo != 0);
		glBindBuffer(GL_ARRAY_BUFFER, model_matrix_vbo);
	}

	void transformer::lock()
	{
		PROFILER_SCOPE_MISC_FN();

		ensure(cached_model_matrices.empty());
		cached_model_matrices.reserve(transforms.size());

		for (transform t : transforms)
		{
			t.lock();
			cached_model_matrices.push_back(t.model_matrix());
		}

		update_model_matrix_vbo();
		_is_locked = true;
	}

	void transformer::unlock()
	{
		ensure(_is_locked, "Can't unlock a transformer that is not locked");
		ensure(model_matrix_vbo != 0);

		cached_model_matrices.clear();

		// Unlock all of the child transforms
		for (transform t : transforms)
			t.unlock();

		_is_locked = false;
		free_the_vbo_buffer();
	}

	void transformer::relock()
	{
		PROFILER_SCOPE_MISC_FN();

		ensure(_is_locked, "Can't relock if the transformer hasn't been locked earlier");

		unlock();
		lock();
	}

	void transformer::update_transform(const size_t index)
	{
		ensure(index < transforms.size());
		ensure(cached_model_matrices.size() == transforms.size());

		transforms[index].unlock();
		transforms[index].lock();
		cached_model_matrices[index] = transforms[index].model_matrix();

#ifndef NDEBUG
		d_singular_transform_updated = true;
#endif
	}

	void transformer::update_vbo_data()
	{
		ensure(model_matrix_vbo != 0);
		bind_vbo();
		glBufferData(GL_ARRAY_BUFFER, cached_model_matrices.size() * sizeof(glm::mat4), cached_model_matrices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

#ifndef NDEBUG
		d_singular_transform_updated = false;
#endif
	}

	bool transformer::is_locked() const
	{
		return _is_locked;
	}

	std::vector<glm::mat4> transformer::model_matrices() const
	{
		PROFILER_SCOPE_MISC_FN();

		if (_is_locked)
			return cached_model_matrices;

		std::vector<glm::mat4> tmp_model_matrices(transforms.size());
		for (size_t i = 0; i < transforms.size(); ++i)
			tmp_model_matrices[i] = transforms[i].model_matrix();

		return tmp_model_matrices;
	}

	void transformer::update_model_matrix_vbo()
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(model_matrix_vbo == 0, "Free the previous VBO before creating a new one");
		ensure(!transforms.empty(), "Can't create a VBO when there are no transforms");

		std::vector<glm::mat4> matrices = model_matrices();

		u32 vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);
		model_matrix_vbo = vbo;

#ifndef NDEBUG
		d_singular_transform_updated = false;
#endif
	}

	void transformer::free_the_vbo_buffer()
	{
		ensure(model_matrix_vbo != 0, "Can't free a VBO that hasn't been allocated yet");
		glDeleteBuffers(1, &model_matrix_vbo);
		model_matrix_vbo = 0;
	}
}

#include "Assert.hpp"
#include "EBO.hpp"
#include "Globals.hpp"
#include "Profiling.hpp"
#include "VAO.hpp"
#include "VBO.hpp"

#include <glad/gl.h>

namespace birb
{
	vao::vao()
	{
		glGenVertexArrays(1, &id);
	}

	vao::~vao()
	{
		ensure(birb::opengl_initialized);
		glDeleteVertexArrays(1, &id);
	}

	void vao::link_vbo(birb::vbo& vbo, u32 layout, u32 num_components, size_t stride, u32 offset)
	{
		PROFILER_SCOPE_RENDER_FN()

		ensure(vbo.id != 0);
		ensure(num_components != 0, "Invalid amount of components");
		ensure(stride != 0 && stride >= num_components, "Invalid stride");

#ifndef NDEBUG
		ensure(vbo.d_vert_count % stride == 0, "Invalid stride");
		d_total_vbo_vert_count += (vbo.d_vert_count / stride) * num_components;
#endif

		vbo.bind();

		glVertexAttribPointer(layout, num_components, GL_FLOAT, GL_FALSE, stride * sizeof(f32), reinterpret_cast<void*>(offset * sizeof(f32)));
		glEnableVertexAttribArray(layout);

		vbo.unbind();
	}

	void vao::link_ebo(birb::ebo& ebo)
	{
		ensure(!has_ebo, "There's already an EBO linked to this VAO");

		ebo.bind();
		has_ebo = true;
	}

	void vao::bind()
	{
		glBindVertexArray(id);
	}

	void vao::unbind()
	{
		glBindVertexArray(0);
	}

	bool vao::contains_ebo() const
	{
		return has_ebo;
	}
}

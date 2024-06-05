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
		ensure(birb::g_opengl_initialized);

		if (id != 0)
			glDeleteVertexArrays(1, &id);
	}

	vao::vao(vao&& other)
	{
		id = other.id;
		other.id = 0;

		has_ebo = other.has_ebo;
		other.has_ebo = false;
	}

	void vao::link_vbo(birb::vbo& vbo, u32 layout, u32 num_components, std::size_t stride, u32 offset)
	{
		PROFILER_SCOPE_RENDER_FN();

		ensure(d_current_bound_vao == id, "Remember to bind the VAO before modifying it");

		ensure(vbo.id != 0);
		ensure(num_components != 0, "Invalid amount of components");
		ensure(stride != 0 && stride >= num_components, "Invalid stride");

		vbo.bind();

		glVertexAttribPointer(layout, num_components, GL_FLOAT, GL_FALSE, stride * sizeof(f32), reinterpret_cast<void*>(offset * sizeof(f32)));
		glEnableVertexAttribArray(layout);

		vbo.unbind();
	}

	void vao::link_ebo(birb::ebo& ebo)
	{
		ensure(d_current_bound_vao == id, "Remember to bind the VAO before modifying it");
		ensure(!has_ebo, "There's already an EBO linked to this VAO");

		ebo.bind();
		has_ebo = true;
	}

	void vao::bind()
	{
		glBindVertexArray(id);

#ifndef NDEBUG
		d_current_bound_vao = id;
#endif
	}

	void vao::unbind()
	{
		glBindVertexArray(0);

#ifndef NDEBUG
		d_current_bound_vao = 0;
#endif
	}

	bool vao::contains_ebo() const
	{
		return has_ebo;
	}
}

#include "Assert.hpp"
#include "GLBuffer.hpp"
#include "Globals.hpp"

#include <glad/gl.h>

namespace birb
{
	static_assert(static_cast<int>(gl_buffer_type::array) == GL_ARRAY_BUFFER);
	static_assert(static_cast<int>(gl_buffer_type::element_array) == GL_ELEMENT_ARRAY_BUFFER);
	static_assert(static_cast<int>(gl_buffer_type::uniform) == GL_UNIFORM_BUFFER);

	gl_buffer::gl_buffer(const gl_buffer_type type)
	:type(type)
	{
		ensure(birb::g_opengl_initialized);
		glGenBuffers(1, &_id);
		ensure(_id != 0);
	}

	gl_buffer::~gl_buffer()
	{
		ensure(birb::g_opengl_initialized);
		if (_id != 0)
			glDeleteBuffers(1, &_id);
	}

	gl_buffer::gl_buffer(gl_buffer&& other)
	:type(other.type)
	{
		this->_id = other._id;
		other._id = 0;
	}

	u32 gl_buffer::id() const
	{
		return _id;
	}

	void gl_buffer::bind() const
	{
		glBindBuffer(static_cast<int>(type), _id);
	}

	void gl_buffer::unbind() const
	{
		glBindBuffer(static_cast<int>(type), 0);
	}

	void gl_buffer::unbind(const gl_buffer_type type)
	{
		glBindBuffer(static_cast<int>(type), 0);
	}
}

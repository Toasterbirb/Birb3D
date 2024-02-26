#include "Renderer.hpp"
#include "Shader.hpp"

#include <cstddef>
#include <cstdint>
#include <glad/gl.h>

// Make sure that all of the datatypes are of correct size
// so that they work correctly with OpenGL
static_assert(sizeof(double) == sizeof(GLdouble));
static_assert(sizeof(float) == sizeof(GLfloat));
static_assert(sizeof(int) == sizeof(GLint));
static_assert(sizeof(long int) == sizeof(GLint64));
static_assert(sizeof(short) == sizeof(GLshort));
static_assert(sizeof(std::byte) == sizeof(GLbyte));
static_assert(sizeof(unsigned int) == sizeof(GLuint));
static_assert(sizeof(unsigned long int) == sizeof(GLuint64));
static_assert(sizeof(unsigned short) == sizeof(GLushort));
static_assert(sizeof(size_t) == sizeof(GLsizeiptr));
static_assert(sizeof(intptr_t) == sizeof(GLintptr));

namespace birb
{
	renderer::renderer()
	{
		event_bus::register_event_id(1, this); // Toggle wireframe mode
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
		event_bus::unregister_event_id(1, this);
	}

	void renderer::process_event(unsigned short event_id, const event_data& data)
	{
		switch (event_id)
		{
			case 1:
				toggle_wireframe();
				break;
		}
	}

	void renderer::draw_elements(vao& vao, size_t index_count)
	{
		vao.bind();
		glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, 0);
	}

	void renderer::draw_arrays(vao& vao, size_t vert_count)
	{
		vao.bind();
		glDrawArrays(GL_TRIANGLES, 0, vert_count);
	}

	void renderer::toggle_wireframe()
	{
		wireframe_mode = !wireframe_mode;

		if (!wireframe_mode)
		{
			birb::log("Toggling wireframe mode off");
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else
		{
			birb::log("Toggling wireframe mode on");
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}

	}
}

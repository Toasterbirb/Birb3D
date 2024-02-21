#include "Renderer.hpp"
#include "Shader.hpp"

#include <cstddef>
#include <cstdint>
#include <glad/gl.h>
#include <vector>

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
	}

	renderer::~renderer()
	{
		birb::log("Crushing the renderer");
	}

	void renderer::draw_verts(std::vector<float> verts)
	{

	}
}

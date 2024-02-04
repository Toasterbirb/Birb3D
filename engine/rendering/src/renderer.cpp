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
		// glDeleteProgram(shader_program);
	}

	void renderer::draw_verts(std::vector<float> verts)
	{
		// glGenVertexArrays(1, &VAO);
		// glGenBuffers(1, &VBO);

		// glBindVertexArray(VAO);

		// glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// glBufferData(GL_ARRAY_BUFFER, 3, verts.data(), GL_STATIC_DRAW);

		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		// glEnableVertexAttribArray(0);

		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		// glBindVertexArray(0);

		// These funcs need to be moved out of here at some point
		// glUseProgram(shader_program);
		// glBindVertexArray(VAO);
		// glDrawArrays(GL_TRIANGLES, 0, 3);


		// TODO: Delete the vertex array and buffers at some point Ow
	}
}

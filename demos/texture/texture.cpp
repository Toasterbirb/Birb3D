#include <Window.hpp>
#include <Renderer.hpp>
#include <vector>
#include <glad/gl.h>
#include <stb_image.h>

#include "Shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

int main(void)
{
	birb::window window("Triangle", birb::vec2<int>(800, 800));
	birb::renderer renderer;

	window.init_imgui();

	// Format: coordinates, colors
	std::vector<float> verts =
	{
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f, 	0.0f, 1.0f, // Upper left corner
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f, 1.0f, 	1.0f, 1.0f, // Upper right corner
		 0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 1.0f, 	1.0f, 0.0f, // Lower right corner
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		0, 3, 2
	};

	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

	birb::shader shader_program("texture_vert.glsl", "texture_frag.glsl");

	birb::vao vao1;
	vao1.bind();

	birb::vbo vbo1(verts);
	birb::ebo ebo1(indices);

	vao1.link_vbo(vbo1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	vao1.link_vbo(vbo1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	vao1.link_vbo(vbo1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	vao1.unbind();
	vbo1.unbind();
	ebo1.unbind();

	unsigned int uni_id = glGetUniformLocation(shader_program.id, "scale");


	birb::vec2<int> texture_size(512, 512);
	stbi_set_flip_vertically_on_load(true);
	int texture_color_channel_count;
	unsigned char* tex_bytes = stbi_load("texture_512.png", &texture_size.x, &texture_size.y, &texture_color_channel_count, 0);

	unsigned int texture;
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_size.x, texture_size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(tex_bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int tex0_uni = glGetUniformLocation(shader_program.id, "tex0");
	shader_program.activate();
	glUniform1i(tex0_uni, 0);

	while (!window.should_close())
	{
		window.clear();

		shader_program.activate();
		glUniform1f(uni_id, 0.5f);
		glBindTexture(GL_TEXTURE_2D, texture);
		vao1.bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window.flip();
		window.poll();
	}

	vao1.unload();
	vbo1.unload();
	ebo1.unload();
	shader_program.unload();

	glDeleteTextures(1, &texture);

	return 0;
}

#include "Assert.hpp"
#include "Color.hpp"
#include "Shader.hpp"
#include "ShaderUniforms.hpp"

#include <glad/gl.h>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

namespace birb
{
	void shader::set(const uniform& uniform, i32 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::INT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform1i(uniform_locations.at(uniform.str(index)), value);
	}

	void shader::set(const uniform& uniform, f32 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::FLOAT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform1f(uniform_locations.at(uniform.str(index)), value);
	}

	void shader::set(const uniform& uniform, const glm::vec3 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::VEC3);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations.at(uniform.str(index)), value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::vec4 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::VEC4);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform4f(uniform_locations.at(uniform.str(index)), value.x, value.y, value.z, value.w);
	}

	void shader::set(const uniform& uniform, const birb::vec3<f32> value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::BIRB_VEC3_FLOAT);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations.at(uniform.str(index)), value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::mat4 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::MAT4);

		add_uniform_location(uniform.str(index));

		activate();
		glUniformMatrix4fv(uniform_locations.at(uniform.str(index)), 1, GL_FALSE, glm::value_ptr(value));
	}

	void shader::set(const uniform& uniform, const color value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::BIRB_COLOR);

		add_uniform_location(uniform.str(index));

		activate();
		glUniform3f(uniform_locations.at(uniform.str(index)), value.r, value.g, value.b);
	}
}

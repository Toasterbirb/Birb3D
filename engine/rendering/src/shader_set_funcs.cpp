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

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_int))
			return;

		activate();
		glUniform1i(key, value);
	}

	void shader::set(const uniform& uniform, f32 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::FLOAT);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_float))
			return;

		activate();
		glUniform1f(key, value);
	}

	void shader::set(const uniform& uniform, const glm::vec2 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::VEC2);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_vec2))
			return;

		activate();
		glUniform2f(key, value.x, value.y);
	}

	void shader::set(const uniform& uniform, const glm::vec3 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::VEC3);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_vec3))
			return;

		activate();
		glUniform3f(key, value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::vec4 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::VEC4);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_vec4))
			return;

		activate();
		glUniform4f(key, value.x, value.y, value.z, value.w);
	}

	void shader::set(const uniform& uniform, const birb::vec3<f32> value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::BIRB_VEC3_FLOAT);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_birb_vec3_float))
			return;

		activate();
		glUniform3f(key, value.x, value.y, value.z);
	}

	void shader::set(const uniform& uniform, const glm::mat4 value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::MAT4);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_mat4))
			return;

		activate();
		glUniformMatrix4fv(key, 1, GL_FALSE, glm::value_ptr(value));
	}

	void shader::set(const uniform& uniform, const color value, i32 index)
	{
		ensure(!uniform.name.empty());
		ensure(uniform.type == uniform_type::BIRB_COLOR);

		const i32 key = uniform_location(uniform.str(index));
		if (uniform_cache(key, value, uniform_cache_color))
			return;

		activate();
		glUniform3f(key, value.r, value.g, value.b);
	}
}

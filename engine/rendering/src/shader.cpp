#include "Shader.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"
#include "ShaderSource.hpp"

#include <array>
#include <cassert>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

namespace birb
{
	shader::shader(const std::string& shader_name)
	:vertex_shader_name(shader_name), fragment_shader_name(shader_name)
	{
		compile_shader(shader_name, shader_name);
	}

	shader::shader(const std::string& vertex, const std::string& fragment)
	:vertex_shader_name(vertex), fragment_shader_name(fragment)
	{
		compile_shader(vertex, fragment);
	}

	shader::~shader()
	{
		glDeleteProgram(this->id);
	}

	void shader::activate()
	{
		glUseProgram(this->id);
	}

	void shader::reset_lights()
	{
		PROFILER_SCOPE_RENDER_FN()

		// Point lights
		for (unsigned int i = 0; i < point_light_count; ++i)
		{
			// Set the lights to black
			set_vec3("point_lights[" + std::to_string(i) + "].ambient", {0.0f, 0.0f, 0.0f});
			set_vec3("point_lights[" + std::to_string(i) + "].diffuse", {0.0f, 0.0f, 0.0f});
			set_vec3("point_lights[" + std::to_string(i) + "].specular", {0.0f, 0.0f, 0.0f});

			// Set the position to zero
			set_vec3( "point_lights[" + std::to_string(i) + "].position", {0.0f, 0.0f, 0.0f});

			// Keep the attenuation values
			set_float("point_lights[" + std::to_string(i) + "].constant", 1.0f);
			set_float("point_lights[" + std::to_string(i) + "].linear", 0.09f);
			set_float("point_lights[" + std::to_string(i) + "].quadratic", 0.032f);
		}

		update_directional_light();
	}

	void shader::update_directional_light()
	{
		PROFILER_SCOPE_RENDER_FN()

		set_vec3("directional_light.direction", directional_direction.to_glm_vec());
		set_vec3("directional_light.ambient", directional_ambient.to_glm_vec());
		set_vec3("directional_light.diffuse", directional_diffuse.to_glm_vec());
		set_vec3("directional_light.specular", directional_specular.to_glm_vec());
	}

	bool shader::has_uniform_var(const std::string& name) const
	{
		return uniform_locations.contains(name);
	}

	void shader::set_mat4(const std::string& name, const glm::mat4 mat4)
	{
		add_uniform_location(name);

		activate();
		glUniformMatrix4fv(uniform_locations[name], 1, GL_FALSE, glm::value_ptr(mat4));
	}

	void shader::set_vec3(const std::string& name, const glm::vec3 vector)
	{
		add_uniform_location(name);

		activate();
		glUniform3f(uniform_locations[name], vector.x, vector.y, vector.z);
	}

	void shader::set_vec4(const std::string& name, const glm::vec4 vector)
	{
		add_uniform_location(name);

		activate();
		glUniform4f(uniform_locations[name], vector.x, vector.y, vector.z, vector.w);
	}

	void shader::set_float(const std::string& name, const float f)
	{
		add_uniform_location(name);

		activate();
		glUniform1f(uniform_locations[name], f);
	}

	void shader::set_int(const std::string& name, const int i)
	{
		add_uniform_location(name);

		activate();
		glUniform1i(uniform_locations[name], i);
	}

	void shader::set_color(const std::string& name, const color& color)
	{
		add_uniform_location(name);

		activate();
		glUniform3f(uniform_locations[name], color.r, color.g, color.b);
	}

	void shader::set_diffuse_color(const color& color)
	{
		diffuse_color = color;
		set_vec3("material.diffuse", { color.r, color.g, color.b });
	}

	void shader::set_specular_color(const color& color)
	{
		specular_color = color;
		set_vec3("material.specular", { color.r, color.g, color.b });
	}

	void shader::set_shininess(const float shininess)
	{
		this->shininess = shininess;
		set_float("material.shininess", shininess);
	}

	void shader::draw_editor_ui()
	{
		PROFILER_SCOPE_RENDER_FN()

		if (ImGui::CollapsingHeader("Shader"))
		{
			assert(!vertex_shader_name.empty());
			assert(!fragment_shader_name.empty());

			ImGui::Text("Vertex: %s", vertex_shader_name.c_str());
			ImGui::Text("Fragment: %s", fragment_shader_name.c_str());

			if (uniform_locations.contains("material.diffuse"))
			{
				if (ImGui::ColorEdit3("Diffuse", *diffuse_color.to_ptr_array().data()))
					set_diffuse_color(diffuse_color);
			}

			if (uniform_locations.contains("material.specular"))
			{
				if (ImGui::ColorEdit3("Specular", *specular_color.to_ptr_array().data()))
					set_specular_color(specular_color);
			}

			if (ImGui::DragFloat("shininess", &shininess, 1.0f, 0.1f, 2048.0f, "%.1f", ImGuiSliderFlags_AlwaysClamp))
				set_shininess(shininess);

			ImGui::Spacing();
			if (ImGui::TreeNode("Uniforms"))
			{
				auto it = uniform_locations.begin();
				while (it != uniform_locations.end())
				{
					ImGui::BulletText("%s", (*it).first.c_str());
					it++;
				}

				ImGui::TreePop();
			}
		}

	}

	void shader::add_uniform_location(const std::string& name)
	{
		assert(!name.empty() && "Empty uniform name");

		// Don't fetch the uniform location if its already in the hashmap
		if (!uniform_locations.contains(name))
		{
			activate();
			int location = glGetUniformLocation(id, name.c_str());

			if (location != -1)
				uniform_locations[name] = location;
			else
				birb::log_warn("Tried to add shader uniform that doesn't exist: " + name);
		}
	}

	void shader::compile_shader(const std::string& vertex, const std::string& fragment)
	{
		assert(!vertex.empty() && "Empty vertex shader name string");
		assert(!fragment.empty() && "Empty fratment shader name string");

		const std::string vertex_name = vertex + "_vert";
		const std::string fragment_name = fragment + "_frag";

		assert(birb::shader_src.contains(vertex_name) && "Tried to use a vertex shader that wasn't in the pregenerated header");
		assert(birb::shader_src.contains(fragment_name) && "Tried to use a fragment shader that wasn't in the pregenerated header");

		const std::string& vertex_src = birb::shader_src[vertex_name];
		const std::string& fragment_src = birb::shader_src[fragment_name];

		const char* vertex_src_c_str = vertex_src.c_str();
		const char* fragment_src_c_str = fragment_src.c_str();

		// Compile shaders
		{
			PROFILER_SCOPE_RENDER("Shader compiling")
			unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(vertex_shader, 1, &vertex_src_c_str, NULL);
			glCompileShader(vertex_shader);
			compile_errors(vertex_shader, "VERTEX");

			unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(fragment_shader, 1, &fragment_src_c_str, NULL);
			glCompileShader(fragment_shader);
			compile_errors(fragment_shader, "FRAGMENT");

			this->id = glCreateProgram();
			glAttachShader(this->id, vertex_shader);
			glAttachShader(this->id, fragment_shader);
			glLinkProgram(this->id);
			compile_errors(id, "PROGRAM");

			// Delete the shaders since they are now in the program
			glDeleteShader(vertex_shader);
			glDeleteShader(fragment_shader);
		}
	}

	void shader::compile_errors(unsigned int shader, const std::string& type)
	{
		constexpr int LOG_BUFFER_SIZE = 1024;

		int has_compiled;
		char info_log[LOG_BUFFER_SIZE];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetShaderInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_error("Shader failed to compile: " + type + " | " + info_log);
			}
		}
		else
		{
			glGetProgramiv(shader, GL_COMPILE_STATUS, &has_compiled);

			if (has_compiled == false)
			{
				glGetProgramInfoLog(shader, LOG_BUFFER_SIZE, NULL, info_log);
				birb::log_error("Shader failed to link: " + type + " | " + info_log);
			}
		}
	}
}

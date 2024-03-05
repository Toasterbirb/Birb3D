#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"

#include <glad/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

namespace birb
{
	class shader : editor_component
	{
	public:
		explicit shader(const std::string& shader_name);
		shader(const std::string& vertex, const std::string& fragment);
		shader(const shader& other);
		~shader();

		// Reference to the shader program
		unsigned int id = 0;

		// Directional lighting
		static inline vec3<float> directional_direction = { 0.2f, 0.2f, -0.7f };
		static inline vec3<float> directional_ambient = { 0.2f, 0.2f, 0.2f };
		static inline vec3<float> directional_diffuse = { 1.0f, 1.0f, 1.0f };
		static inline vec3<float> directional_specular = { 1.0f, 1.0f, 1.0f };

		// Activate the shader program
		void activate();

		/**
		 * @brief Set all directional and point lights to black
		 */
		void reset_lights();

		/**
		 * @brief Add the following uniforms to the shader:
		 * - model
		 * - view
		 * - projection
		 */
		void add_default_3d_matrix_uniforms();

		void update_directional_light();

		bool has_uniform_var(const std::string& name) const;

		void set_mat4(const std::string& name, const glm::mat4 mat4);
		void set_vec3(const std::string& name, const glm::vec3 vector);
		void set_vec4(const std::string& name, const glm::vec4 vector);
		void set_float(const std::string& name, const float f);
		void set_int(const std::string& name, const int i);
		void set_color(const std::string& name, const color& color);

		// Material helper functions
		void set_diffuse_color(const color& color);
		void set_specular_color(const color& color);
		void set_shininess(const float shininess);
		void apply_color_material();

		void draw_editor_ui() override;

		// Shader source code related functions
		static std::vector<std::string> vertex_shader_name_list();
		static std::vector<std::string> fragment_shader_name_list();

	private:
		void add_uniform_location(const std::string& name);
		void compile_shader(const std::string& vertex, const std::string& fragment);
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, int> uniform_locations;

		const unsigned int point_light_count = 4;
		std::string vertex_shader_name = "NULL";
		std::string fragment_shader_name = "NULL";

		// Material variables
		float shininess = 32.0f;
		color diffuse_color = { 0.0f, 0.0f, 0.0f };
		color specular_color = { 0.0f, 0.0f, 0.0f };
	};
}

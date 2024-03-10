#pragma once

#include "Color.hpp"
#include "EditorComponent.hpp"
#include "Vector.hpp"

#include <glad/gl.h>
#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

namespace birb
{
	struct point_light
	{
		std::string name = "";
		vec3<float> ambient = { 0.0f, 0.0f, 0.0f };
		vec3<float> diffuse = { 0.0f, 0.0f, 0.0f };
		vec3<float> specular = { 0.0f, 0.0f, 0.0f };

		vec3<float> position = { 0.0f, 0.0f, 0.0f };

		float attenuation_constant = 1.0f;
		float attenuation_linear = 0.09f;
		float attenuation_quadratic = 0.032f;
	};

	class shader : editor_component
	{
	public:
		shader();
		explicit shader(const std::string& shader_name);
		shader(const std::string& vertex, const std::string& fragment);
		shader(const shader& other);
		~shader();

		// Reference to the shader program
		unsigned int id = 0;

		// Lighting constants
		const static unsigned short point_light_count = 4;

		// Directional lighting
		static inline vec3<float> directional_direction = { 0.2f, 0.2f, -0.7f };
		static inline vec3<float> directional_ambient = { 0.2f, 0.2f, 0.2f };
		static inline vec3<float> directional_diffuse = { 1.0f, 1.0f, 1.0f };
		static inline vec3<float> directional_specular = { 1.0f, 1.0f, 1.0f };

		// Point lights
		static inline std::array<point_light, point_light_count> point_lights;

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
		void update_point_lights();

		bool has_uniform_var(const std::string& name) const;

		void set_mat4(const std::string& name, const glm::mat4 mat4);
		void set_vec3(const std::string& name, const glm::vec3 vector);
		void set_vec3_birb_float(const std::string& name, const vec3<float> vector);
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
		void compile();
		static std::vector<std::string> vertex_shader_name_list();
		static std::vector<std::string> fragment_shader_name_list();

		// Shader caching
		static void clear_shader_cache();
		static size_t shader_cache_size();
		static size_t shader_cache_hits();

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(
				vertex_shader_name,
				fragment_shader_name,
				shininess,
				diffuse_color,
				specular_color
			);
		}

	private:
		void add_uniform_location(const std::string& name);
		void compile_shader(const std::string& vertex, const std::string& fragment);
		void compile_errors(unsigned int shader, const std::string& type);
		std::unordered_map<std::string, int> uniform_locations;

		std::string vertex_shader_name = "NULL";
		std::string fragment_shader_name = "NULL";

		// Material variables
		float shininess = 32.0f;
		color diffuse_color = { 0.0f, 0.0f, 0.0f };
		color specular_color = { 0.0f, 0.0f, 0.0f };
	};
}

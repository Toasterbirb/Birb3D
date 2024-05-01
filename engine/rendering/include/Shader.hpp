#pragma once

#include "EditorComponent.hpp"
#include "Vector.hpp"

#include <glm/fwd.hpp>
#include <string>
#include <unordered_map>

namespace birb
{
	struct color;
	struct uniform;

	namespace component
	{
		struct material;
	}

	struct point_light
	{
		std::string name = "";
		vec3<f32> ambient = { 0.0f, 0.0f, 0.0f };
		vec3<f32> diffuse = { 0.0f, 0.0f, 0.0f };
		vec3<f32> specular = { 0.0f, 0.0f, 0.0f };

		vec3<f32> position = { 0.0f, 0.0f, 0.0f };

		f32 attenuation_constant = 1.0f;
		f32 attenuation_linear = 0.09f;
		f32 attenuation_quadratic = 0.032f;
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
		u32 id = 0;

		// Lighting constants
		const static u16 point_light_count = 4;

		// Directional lighting
		static inline vec3<f32> directional_direction = { 0.2f, 0.2f, -0.7f };
		static inline vec3<f32> directional_ambient = { 0.2f, 0.2f, 0.2f };
		static inline vec3<f32> directional_diffuse = { 1.0f, 1.0f, 1.0f };
		static inline vec3<f32> directional_specular = { 1.0f, 1.0f, 1.0f };

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

		// These functions are defined in shader_set_funcs.cpp instead of shader.cpp
		void set(const uniform& uniform, i32 value, i32 index = -1);
		void set(const uniform& uniform, f32 value, i32 index = -1);
		void set(const uniform& uniform, const glm::vec3 value, i32 index = -1);
		void set(const uniform& uniform, const glm::vec4 value, i32 index = -1);
		void set(const uniform& uniform, const birb::vec3<f32> value, i32 index = -1);
		void set(const uniform& uniform, const glm::mat4 value, i32 index = -1);
		void set(const uniform& uniform, const color value, i32 index = -1);

		void set_int(const std::string& name, const i32 value);

		void apply_color_material(const component::material& material);

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

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
				fragment_shader_name
			);
		}

	private:
		static inline const std::string editor_header_name = "Shader";

		// Calls try_add_uniform_location but prints out a warning if the uniform
		// doesn't exist
		void add_uniform_location(const std::string& name);

		// Try to add a new uniform location without caring if it exists or not
		// The return value will be -1 if it didn't exist and something other than -1
		// if it did exists
		i32 try_add_uniform_location(const std::string& name);

		// Material helper functions and variables
		void set_diffuse_color(const color& color);
		void set_specular_color(const color& color);
		void set_shininess(const f32 shininess);

		void compile_shader(const std::string& vertex, const std::string& fragment);
		void compile_errors(u32 shader, const std::string& type);
		std::unordered_map<std::string, i32> uniform_locations;

		std::string vertex_shader_name = "NULL";
		std::string fragment_shader_name = "NULL";
	};
}

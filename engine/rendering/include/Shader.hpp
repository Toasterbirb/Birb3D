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
	struct material;

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

		void update_directional_light();
		void update_point_lights();

		bool has_uniform_var(const std::string& name) const;
		i32 uniform_location(const std::string& name) const;

		// These functions are defined in shader_set_funcs.cpp instead of shader.cpp
		void set(const uniform& uniform, i32 value, i32 index = -1);
		void set(const uniform& uniform, f32 value, i32 index = -1);
		void set(const uniform& uniform, const glm::vec2 value, i32 index = -1);
		void set(const uniform& uniform, const glm::vec3 value, i32 index = -1);
		void set(const uniform& uniform, const glm::vec4 value, i32 index = -1);
		void set(const uniform& uniform, const birb::vec3<f32> value, i32 index = -1);
		void set(const uniform& uniform, const glm::mat4 value, i32 index = -1);
		void set(const uniform& uniform, const color value, i32 index = -1);

		void set_int(const std::string& name, const i32 value);

		void apply_color_material(const material& material);

		void draw_editor_ui() override;
		std::string collapsing_header_name() const override;

		// Shader source code related stuff
		void compile();
		static std::vector<std::string> vertex_shader_name_list();
		static std::vector<std::string> fragment_shader_name_list();

		/**
		 * @brief Directories that get checked for shader source code
		 *
		 * This variable gets used when a shader needs to be compiled
		 * but the source code for it hasn't been built into the engine
		 *
		 * The directory paths are relative to the working directory
		 */
		static inline std::vector<std::string> shader_src_search_paths;

		bool is_missing() const;

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

		// Material helper functions and variables
		void set_diffuse_color(const color& color);
		void set_specular_color(const color& color);
		void set_shininess(const f32 shininess);

		std::string load_shader_src(const std::string& shader_name) const;
		bool _is_missing = false;

		// OpenGL shader type enums as integers
		enum shader_type
		{
			program = 0,
			vertex = 35633,
			fragment = 35632,
		};

		void compile_shader(const std::string& vertex, const std::string& fragment);
		u32 compile_gl_shader_program(const std::string& shader_name, const char* shader_src, const shader_type type);
		std::string shader_type_to_str(const shader_type type) const;
		void compile_errors(u32 shader, const shader_type type);

		std::string vertex_shader_name = "NULL";
		std::string fragment_shader_name = "NULL";

		// Uniform variable cache
		// Returns true if the variable was cached
		template<typename T>
		__attribute__((always_inline))
		constexpr inline bool uniform_cache(const i32 key, const T value, std::unordered_map<i32, T>& cache)
		{
			if (cache[key] == value)
				return true;

			cache[key] = value;
			return false;
		}

		std::unordered_map<i32, i32> uniform_cache_int;
		std::unordered_map<i32, f32> uniform_cache_float;
		std::unordered_map<i32, glm::vec2> uniform_cache_vec2;
		std::unordered_map<i32, glm::vec3> uniform_cache_vec3;
		std::unordered_map<i32, vec3<f32>> uniform_cache_birb_vec3_float;
		std::unordered_map<i32, glm::vec4> uniform_cache_vec4;
		std::unordered_map<i32, glm::mat4> uniform_cache_mat4;
		std::unordered_map<i32, color> uniform_cache_color;
	};
}

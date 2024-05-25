#pragma once

#include "Types.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace birb
{
	enum class uniform_type
	{
		INT,
		FLOAT,
		VEC2,
		VEC3,
		VEC4,
		MAT4,
		BIRB_VEC3_FLOAT,
		BIRB_COLOR,
	};

	struct uniform
	{
		uniform(const std::string& name, uniform_type type, const std::string& struct_var = "", bool is_array = false, i32 array_max_size = 0);

		std::string name;
		const uniform_type type;
		const bool is_array = false;
		const i32 array_max_size;

		std::vector<std::string> array_str_cache;

		std::string str(i32 index = -1) const;
	};

	struct uniform_block
	{
		uniform_block(const u32 bind_point, const char* block_name, const size_t size, const bool static_draw);
		const u32 bind_point;
		const char* block_name;
		const size_t size;
		const bool static_draw;
	};

	namespace shader_uniforms
	{
		const static inline uniform model("model", uniform_type::MAT4);
		const static inline uniform view("view", uniform_type::MAT4);
		const static inline uniform projection("projection", uniform_type::MAT4);
		const static inline uniform view_pos("view_pos", uniform_type::VEC3);

		const static inline uniform color("color", uniform_type::BIRB_COLOR);
		const static inline uniform text_color("text_color", uniform_type::BIRB_COLOR);
		const static inline uniform text_position("text_position", uniform_type::BIRB_VEC3_FLOAT);

		namespace block
		{
			const static inline uniform_block view_matrices(0, "view_matrices", sizeof(glm::mat4), true);
			const static inline uniform_block projection_matrices(1, "projection_matrices", sizeof(glm::mat4) * 3, true);
		}

		namespace lights
		{
			const static inline uniform light_space_matrix("light_space_matrix", uniform_type::MAT4);
			const static inline uniform shadow_map("shadow_map", uniform_type::INT);
		}

		namespace point_lights
		{
			const static inline uniform position ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "position", true, 64);
			const static inline uniform constant ("point_lights", uniform_type::FLOAT, "constant", true, 64);
			const static inline uniform linear ("point_lights", uniform_type::FLOAT, "linear", true, 64);
			const static inline uniform quadratic ("point_lights", uniform_type::FLOAT, "quadratic", true, 64);
			const static inline uniform ambient ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "ambient", true, 64);
			const static inline uniform diffuse ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "diffuse", true, 64);
			const static inline uniform specular ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "specular", true, 64);
			const static inline uniform count ("point_light_count", uniform_type::INT);
		}

		namespace directional_light
		{
			const static inline uniform direction("directional_light", uniform_type::BIRB_VEC3_FLOAT, "direction", false);
			const static inline uniform ambient("directional_light", uniform_type::BIRB_VEC3_FLOAT, "ambient", false);
			const static inline uniform diffuse("directional_light", uniform_type::BIRB_VEC3_FLOAT, "diffuse", false);
			const static inline uniform specular("directional_light", uniform_type::BIRB_VEC3_FLOAT, "specular", false);
		}

		namespace material
		{
			const static inline uniform type("material_type", uniform_type::INT);
		}

		namespace material_color
		{
			const static inline uniform diffuse("material", uniform_type::BIRB_COLOR, "diffuse", false);
			const static inline uniform specular("material", uniform_type::BIRB_COLOR, "specular", false);
			const static inline uniform shininess("material", uniform_type::FLOAT, "shininess", false);
		}

		namespace material_texture
		{
			const static inline uniform diffuse("material", uniform_type::INT, "diffuse_tex", false);
			const static inline uniform specular("material", uniform_type::INT, "specular_tex", false);
			const static inline uniform shininess("material", uniform_type::FLOAT, "shininess", false);
		}

		namespace texture_units
		{
			const static inline uniform tex0("tex0", uniform_type::INT);
		}

		namespace texture
		{
			const static inline uniform instanced("instanced", uniform_type::INT);
			const static inline uniform aspect_ratio("aspect_ratio", uniform_type::VEC2);
			// const static inline uniform aspect_ratio_reverse("aspect_ratio_reverse", uniform_type::FLOAT);
		}
	}
}

#pragma once

#include <string>

namespace birb
{
	enum class uniform_type
	{
		INT,
		FLOAT,
		VEC3,
		VEC4,
		MAT4,
		BIRB_VEC3_FLOAT,
		BIRB_COLOR,
	};

	struct uniform
	{
		uniform(const std::string& name, uniform_type type, const std::string& struct_var = "", bool is_array = false)
		:name(name), type(type), struct_var(struct_var), is_array(is_array)
		{}

		std::string name;
		uniform_type type;
		std::string struct_var;
		bool is_array = false;

		std::string str(i32 index = -1) const
		{
			std::string str = name;

			assert((!is_array && index == -1) || (is_array && index >= 0));

			if (is_array)
				str += "[" + std::to_string(index) + "]";

			if (!struct_var.empty())
				str += "." + struct_var;

			return str;
		}
	};

	namespace shader_uniforms
	{
		const static inline uniform model("model", uniform_type::MAT4);
		const static inline uniform view("view", uniform_type::MAT4);
		const static inline uniform projection("projection", uniform_type::MAT4);
		const static inline uniform view_pos("view_pos", uniform_type::VEC3);

		const static inline uniform color("color", uniform_type::BIRB_COLOR);

		namespace point_lights
		{
			const static inline uniform position ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "position", true);
			const static inline uniform constant ("point_lights", uniform_type::FLOAT, "constant", true);
			const static inline uniform linear ("point_lights", uniform_type::FLOAT, "linear", true);
			const static inline uniform quadratic ("point_lights", uniform_type::FLOAT, "quadratic", true);
			const static inline uniform ambient ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "ambient", true);
			const static inline uniform diffuse ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "diffuse", true);
			const static inline uniform specular ("point_lights", uniform_type::BIRB_VEC3_FLOAT, "specular", true);
		}

		namespace directional_light
		{
			const static inline uniform direction("directional_light", uniform_type::BIRB_VEC3_FLOAT, "direction", false);
			const static inline uniform ambient("directional_light", uniform_type::BIRB_VEC3_FLOAT, "ambient", false);
			const static inline uniform diffuse("directional_light", uniform_type::BIRB_VEC3_FLOAT, "diffuse", false);
			const static inline uniform specular("directional_light", uniform_type::BIRB_VEC3_FLOAT, "specular", false);
		}

		namespace material_color
		{
			const static inline uniform diffuse("material", uniform_type::BIRB_COLOR, "diffuse", false);
			const static inline uniform specular("material", uniform_type::BIRB_COLOR, "specular", false);
			const static inline uniform shininess("material", uniform_type::FLOAT, "shininess", false);
		}

		namespace material_texture
		{
			const static inline uniform diffuse("material", uniform_type::INT, "diffuse", false);
			const static inline uniform specular("material", uniform_type::INT, "specular", false);
			const static inline uniform shininess("material", uniform_type::FLOAT, "shininess", false);
		}

		namespace texture_units
		{
			const static inline uniform tex0("tex0", uniform_type::INT);
		}

		namespace texture
		{
			const static inline uniform aspect_ratio("aspect_ratio", uniform_type::FLOAT);
			const static inline uniform aspect_ratio_reverse("aspect_ratio_reverse", uniform_type::FLOAT);
		}
	}
}

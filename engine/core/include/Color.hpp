#pragma once

#include "Assert.hpp"
#include "Types.hpp"

#include <array>
#include <iostream>
#include <string>

namespace birb
{
	template <class T>
	struct vec3;

	struct color
	{
		f32 r; ///< red
		f32 g; ///< green
		f32 b; ///< blue
		f32 a; ///< alpha

		/**
		 * @brief Construct a color with all values set to zero
		 */
		constexpr color() : r(0), g(0), b(0), a(0) {}

		/**
		 * @brief Construct a color from floating point values
		 *
		 * All of the parameters given should be between 0.0f and 1.0f.
		 * Any values beyond those limits will result in undefined behavior.
		 *
		 * @param r red
		 * @param g green
		 * @param b blue
		 * @param a alpha
		 */
		constexpr color(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}

		/**
		 * @brief Construct a color from floating point values
		 *
		 * All of the parameters given should be between 0.0 and 1.0.
		 * Any values beyond those limits will result in undefined behavior.
		 *
		 * @param r red
		 * @param g green
		 * @param b blue
		 * @param a alpha
		 */
		constexpr color(f64 r, f64 g, f64 b, f64 a = 1.0) : r(r), g(g), b(b), a(a) {}

		/**
		 * @brief Construct a color from integer values
		 *
		 * All of the parameters given should be between 0 and 255.
		 * Any values beyond those limits will result in undefined behavior.
		 *
		 * @param r red
		 * @param g green
		 * @param b blue
		 * @param a alpha
		 */
		color(i32 r, i32 g, i32 b, i32 a = 255)
		{
			ensure(r >= 0 && r <= 255);
			ensure(g >= 0 && r <= 255);
			ensure(b >= 0 && b <= 255);
			ensure(a >= 0 && a <= 255);

			constexpr f32 reverse_division = 1 / 255.0f;
			this->r = r * reverse_division;
			this->g = g * reverse_division;
			this->b = b * reverse_division;
			this->a = a * reverse_division;
		}

		/**
		 * @brief Construct a color from a hex value
		 *
		 * @note The amount of alpha will always be set to 1.0
		 *
		 * @param hex Hex encoded RGB value (i.e. 0xFF00FF)
		 */
		constexpr color(i32 hex)
		:a(1.0f)
		{
			constexpr f32 reverse_division = 1 / 255.0f;
			this->r = ((hex & 0xff0000) >> 16) * reverse_division;
			this->g = ((hex & 0xff00) >> 8) * reverse_division;
			this->b = (hex & 0xff) * reverse_division;
		}

		/**
		 * @return Color in the following string form: "[r, g, b, a]"
		 */
		std::string to_string() const
		{
			return "[" +
				std::to_string(r) + ", " +
				std::to_string(g) + ", " +
				std::to_string(b) + ", " +
				std::to_string(a) +
				"]";
		}

		std::array<f32*, 3> to_ptr_array()
		{
			return { &r, &g, &b };
		}

		std::array<f32*, 4> to_ptr_array_alpha()
		{
			return { &r, &g, &b, &a };
		}

		vec3<f32> to_vec3();

		constexpr bool operator==(const color& other) const
		{
			return 	(this->r == other.r
					&& this->g == other.g
					&& this->b == other.b
					&& this->a == other.a);
		}

		constexpr bool operator!=(const color& other) const
		{
			return 	(this->r != other.r
					|| this->g != other.g
					|| this->b != other.b
					|| this->a != other.a);
		}

		template<class Archive>
		void serialize(Archive& ar)
		{
			ar(r, g, b, a);
		}
	};

	template<typename T>
	std::ostream &operator<<(std::ostream &stream, const color &other)
    {
        stream << "[" << other.r << ", " << other.g << ", " << other.b << ", " << other.a << "]";
        return stream;
    }
}

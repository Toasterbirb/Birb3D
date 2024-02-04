#pragma once

namespace birb
{
	struct color
	{
		float r, g, b, a;

		constexpr color() : r(0), g(0), b(0), a(0) {}
		constexpr color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
		constexpr color(double r, double g, double b, double a = 1.0) : r(r), g(g), b(b), a(a) {}

		constexpr color(int r, int g, int b, int a = 255)
		{
			constexpr float reverse_division = 1 / 255.0f;
			this->r = r * reverse_division;
			this->g = g * reverse_division;
			this->b = b * reverse_division;
			this->a = a * reverse_division;
		}

		// Construct a color from a hex value
		constexpr color(int hex)
		:a(1.0f)
		{
			constexpr float reverse_division = 1 / 255.0f;
			this->r = ((hex & 0xff0000) >> 16) * reverse_division;
			this->g = ((hex & 0xff00) >> 8) * reverse_division;
			this->b = (hex & 0xff) * reverse_division;
		}

		// Returns the color in the following string form: "[r, g, b, a]"
		std::string to_string() const
		{
			return "[" +
				std::to_string(r) + ", " +
				std::to_string(g) + ", " +
				std::to_string(b) + ", " +
				std::to_string(a) +
				"]";
		}

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
	};

	template<typename T>
	std::ostream &operator<<(std::ostream &stream, const color &other)
    {
        stream << "[" << other.r << ", " << other.g << ", " << other.b << ", " << other.a << "]";
        return stream;
    }
}

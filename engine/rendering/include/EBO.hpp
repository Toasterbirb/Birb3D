#pragma once

#include <array>
#include <vector>

namespace birb
{
	// Element array buffer
	class ebo
	{
	public:
		explicit ebo(const std::vector<unsigned int>& indices);

		template<size_t N>
		explicit ebo(const std::array<unsigned int, N>& indices)
		{
			static_assert(N != 0, "Empty index array");
			static_assert(N < 33000, "You might wanna check the index count on that model");

			load(indices.data(), indices.size());
		}

		ebo(const ebo&) = delete;
		~ebo();

		// Reference to the element array buffer
		unsigned int id = 0;

		void bind();
		void unbind();

	private:
		void load(const unsigned int* indices, const size_t size);
	};
}

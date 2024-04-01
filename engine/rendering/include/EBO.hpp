#pragma once

#include <array>
#include <vector>

namespace birb
{
	// Element array buffer
	class ebo
	{
	public:
		explicit ebo(const std::vector<u32>& indices);

		template<size_t N>
		explicit ebo(const std::array<u32, N>& indices)
		{
			static_assert(N != 0, "Empty index array");
			static_assert(N < 33000, "You might wanna check the index count on that model");

			load(indices.data(), indices.size());
		}

		ebo(const ebo&) = delete;
		~ebo();

		// Reference to the element array buffer
		u32 id = 0;

		void bind();
		void unbind();

	private:
		void load(const u32* indices, const size_t size);
	};
}

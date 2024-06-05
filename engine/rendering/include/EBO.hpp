#pragma once

#include "GLBuffer.hpp"
#include "Types.hpp"

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
		:buffer(gl_buffer_type::element_array)
		{
			static_assert(N != 0, "Empty index array");
			static_assert(N < 33000, "You might wanna check the index count on that model");

			load(indices.data(), indices.size());
		}

		~ebo() = default;
		ebo(ebo&) = delete;
		ebo(const ebo&) = delete;
		ebo(ebo&&) = default;

		// Reference to the element array buffer
		u32 id() const;

		void bind();
		void unbind();

	private:
		void load(const u32* indices, const size_t size);
		gl_buffer buffer;
	};
}

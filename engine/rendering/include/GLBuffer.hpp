#pragma once

#include "Types.hpp"

namespace birb
{
	enum class gl_buffer_type
	{
		array = 34962,
		element_array = 34963,
		uniform = 35345,
	};

	class gl_buffer
	{
	public:
		gl_buffer(const gl_buffer_type type);
		~gl_buffer();
		gl_buffer(const gl_buffer&) = delete;
		gl_buffer(gl_buffer&) = delete;
		gl_buffer(gl_buffer&& other);

		u32 id() const;
		void bind() const;
		void unbind() const;
		static void unbind(const gl_buffer_type type);

		constexpr operator u32()
		{
			return _id;
		}

	private:
		u32 _id;
		const gl_buffer_type type;
	};
}

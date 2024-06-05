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

	enum class gl_usage
	{
		static_draw = 35044,
		dynamic_draw = 35048,
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

		/**
		 * @brief Create and initialize the data store of the buffer
		 *
		 * @param size Size of the data passed in (in bytes)
		 * @param data Pointer to the new data
		 * @param usage Expected usage pattern
		 */
		void set_data(const std::size_t size, const void* data, const gl_usage usage) const;

		/**
		 * @brief Update a portion of the buffer
		 *
		 * @param size The size of the region to be modified (in bytes)
		 * @param data Pointer to the new data
		 * @param offset Offset from the beginning of the buffer to the data region
		 *               that will be modified
		 */
		void update_data(const std::size_t size, const void* data, const u32 offset) const;

		constexpr operator u32()
		{
			return _id;
		}

	private:
		u32 _id;
		const gl_buffer_type type;
	};
}

#pragma once

#include "GLBuffer.hpp"
#include "ShaderUniforms.hpp"
#include "Types.hpp"

#include <cstdint>

namespace birb
{
	class ubo
	{
	public:
		ubo(const uniform_block& block);
		ubo(const u32 bind_point, const u32 size_in_bytes, const bool static_draw);
		~ubo() = default;
		ubo(const ubo&) = delete;
		ubo(ubo&) = delete;
		ubo(ubo&&) = default;

		void bind() const;
		static void unbind();
		void update_data(const void* data, const std::intptr_t size, const std::intptr_t offset) const;

	private:
		void create_ubo(const u32 bind_point, const u32 size_in_bytes, const bool static_draw);

		gl_buffer buffer;
		u32 size = 0;
	};
}

#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace birb
{
	class audio_device
	{
	public:
		audio_device();
		~audio_device();
		audio_device(const audio_device&) = default;
		audio_device(audio_device&) = default;

	private:
		ALCdevice* device = nullptr;
		ALCcontext* context = nullptr;
	};
}

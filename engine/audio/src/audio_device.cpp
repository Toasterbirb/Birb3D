#include "Assert.hpp"
#include "AudioDevice.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"

#include <sndfile.h>

static inline bool alc_is_init = false;

namespace birb
{
	audio_device::audio_device()
	{
		PROFILER_SCOPE_AUDIO_FN()

		ensure(alc_is_init == false, "You shouldn't need to initialize OpenAL more than once");
		alc_is_init = true;

		ALenum error;

		device = alcOpenDevice(NULL);

		if (device)
		{
			context = alcCreateContext(device, NULL);
			alcMakeContextCurrent(context);
		}
		else
		{
			if ((error = alGetError()) != AL_NO_ERROR)
				birb::log_error("alcDevice ", error);

			return;
		}

		// TODO: Check for EAX 2.0 support
	}

	audio_device::~audio_device()
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(context);
		alcCloseDevice(device);
	}
}

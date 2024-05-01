#include "AudioSource.hpp"
#include "Profiling.hpp"

#include <AL/al.h>
#include <cassert>

namespace birb
{
	audio_source::audio_source()
	{
		PROFILER_SCOPE_AUDIO_FN()

		alGenSources(1, &source);
		assert(alGetError() == AL_NO_ERROR && "Failed to setup sound source");
	}

	audio_source::~audio_source()
	{
		alDeleteSources(1, &source);
		assert(alGetError() == AL_NO_ERROR && "Failed delete and audio source");
	}

	void audio_source::play_sound(sound_file& sound_file)
	{
		PROFILER_SCOPE_AUDIO_FN()

		alSourcei(source, AL_BUFFER, static_cast<ALint>(sound_file.buffer()));
		alSourcePlay(source);
	}

	bool audio_source::is_playing() const
	{
		ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}
}

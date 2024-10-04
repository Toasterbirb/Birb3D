#include "ALErrorHandler.hpp"
#include "Assert.hpp"
#include "AudioSource.hpp"
#include "Logger.hpp"
#include "Profiling.hpp"

#include <AL/al.h>

namespace birb
{
	audio_source::audio_source()
	{
		PROFILER_SCOPE_AUDIO_FN();

		alGenSources(1, &source);
		ensure(alGetError() == AL_NO_ERROR, "Failed to setup sound source");
	}

	audio_source::~audio_source()
	{
		ensure(alGetError() == AL_NO_ERROR);

		alDeleteSources(1, &source);
		auto al_error = alGetError();

		if (al_error != AL_NO_ERROR)
			log_al_error_message(al_error);

		// ensure(alGetError() == AL_NO_ERROR, "Failed to delete an audio source");
	}

	void audio_source::play_sound(sound_file& sound_file)
	{
		PROFILER_SCOPE_AUDIO_FN();

		ensure(alGetError() == AL_NO_ERROR);

		alSourcei(source, AL_BUFFER, static_cast<ALint>(sound_file.buffer()));
		alSourcePlay(source);
		attached_audio_buffers.emplace_back(sound_file.buffer());
		check_al_errors();
	}

	void audio_source::unqueue_buffers()
	{
		// don't do anything if the source is playing audio
		// otherwise we might risk unqueueing something that is still playing
		if (is_playing())
			return;

		alSourceUnqueueBuffers(source, attached_audio_buffers.size(), attached_audio_buffers.data());
	}

	bool audio_source::is_playing() const
	{
		ALenum state;
        alGetSourcei(source, AL_SOURCE_STATE, &state);
		ensure(alGetError() == AL_NO_ERROR, "Error when checking if audio is playing");
		return state == AL_PLAYING;
	}
}

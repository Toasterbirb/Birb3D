#include "ALErrorHandler.hpp"
#include "Assert.hpp"
#include "AudioPlayer.hpp"
#include "Profiling.hpp"

namespace birb
{
	audio_player::audio_player()
	{
		// Allocate at least one audio source by default
		audio_sources.push_back(std::make_shared<audio_source>());
	}

	audio_player::~audio_player()
	{
		unqueue_buffers();
	}

	void audio_player::play_sound(sound_file& sound_file)
	{
		PROFILER_SCOPE_AUDIO_FN();

		check_al_errors();

		// Find an audio source that isn't playing anything at the moment
		// If a free source is found, reuse it
		for (std::shared_ptr<audio_source> source : audio_sources)
		{
			if (!source->is_playing())
			{
				source->play_sound(sound_file);
				return;
			}
		}

		ensure(audio_sources.size() < 255, "Audio source count limit reached");

		// No free source was found, so we'll need to allocate a new one
		audio_sources.push_back(std::make_shared<audio_source>());

		// apply the current default volume
		alSourcef(audio_sources.at(audio_sources.size() - 1)->id(), AL_GAIN, current_default_volume);

		audio_sources.at(audio_sources.size() - 1)->play_sound(sound_file);

		check_al_errors();
	}

	void audio_player::unqueue_buffers()
	{
		for (auto source : audio_sources)
			source->unqueue_buffers();
	}

	void audio_player::free_sources()
	{
		audio_sources.clear();
	}

	void audio_player::set_global_source_volume(const f32 new_volume)
	{
		current_default_volume = new_volume;
		for (auto source : audio_sources)
			alSourcef(source->id(), AL_GAIN, new_volume);
	}
}

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

	void audio_player::play_sound(sound_file& sound_file)
	{
		PROFILER_SCOPE_AUDIO_FN();

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
		audio_sources.at(audio_sources.size() - 1)->play_sound(sound_file);
	}
}

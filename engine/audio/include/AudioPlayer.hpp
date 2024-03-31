#pragma once

#include "AudioSource.hpp"
#include "SoundFile.hpp"

#include <memory>
#include <vector>

namespace birb
{
	class audio_player
	{
	public:
		audio_player();
		~audio_player() = default;
		audio_player(const audio_player&) = default;
		audio_player(audio_player&) = default;

		/**
		 * @brief Play a sound with an audio source
		 *
		 * Tries to re-use previously allocated sources.
		 * If non can be found, a new one will be allocated.
		 */
		void play_sound(sound_file& sound_file);

	private:
		std::vector<std::shared_ptr<audio_source>> audio_sources;
	};
}

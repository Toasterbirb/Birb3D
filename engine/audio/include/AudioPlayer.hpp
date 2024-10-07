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
		~audio_player();
		audio_player(const audio_player&) = default;
		audio_player(audio_player&) = default;

		/**
		 * @brief Play a sound with an audio source
		 *
		 * Tries to re-use previously allocated sources.
		 * If non can be found, a new one will be allocated.
		 */
		void play_sound(sound_file& sound_file);
		void unqueue_buffers();
		void free_sources();

		/**
		 * @brief Apply a new volume value to all sources at once
		 */
		void set_global_source_volume(const f32 new_volume);

	private:
		f32 current_default_volume{1.0f};
		std::vector<std::shared_ptr<audio_source>> audio_sources;
	};
}

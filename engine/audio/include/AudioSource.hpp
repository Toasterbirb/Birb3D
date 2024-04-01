#pragma once

#include "SoundFile.hpp"

namespace birb
{
	class audio_source
	{
	public:
		audio_source();
		~audio_source();
		audio_source(const audio_source&) = delete;
		audio_source(audio_source&) = delete;

		void play_sound(sound_file& sound_file);
		bool is_playing() const;

	private:
		u32 source = 0;
	};
}

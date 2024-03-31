#pragma once

#include <AL/al.h>
#include <sndfile.h>
#include <string>

namespace birb
{
	class sound_file
	{
	public:
		sound_file();
		explicit sound_file(const std::string& file_path);
		~sound_file();
		sound_file(const sound_file&) = default;
		sound_file(sound_file&) = default;

		/**
		 * @brief Load a soundfile from a given file path
		 *
		 * @param file_path
		 * @return True if the sound file was loaded successfully
		 */
		bool load(const std::string& file_path);

		/**
		 * @brief Returns the OpenAL audio buffer
		 */
		unsigned int buffer();

	private:
		SF_INFO sfinfo;
		ALenum format;
		sf_count_t frame_count;
		ALsizei byte_count;
		ALuint audio_buffer = 0;

		int splblockalign = 0;
		int byteblockalign = 0;
	};
}

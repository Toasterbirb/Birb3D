#include "Logger.hpp"
#include "Profiling.hpp"
#include "SoundFile.hpp"

#include <AL/alext.h>
#include <cassert>
#include <climits>
#include <memory>

static_assert(sizeof(int) == sizeof(ALint));

namespace birb
{
	struct free_delete
	{
		void operator()(void* x)
		{
			free(x);
		}
	};

	sound_file::sound_file() {}

	sound_file::sound_file(const std::string& file_path)
	{
		load(file_path);
	}

	sound_file::~sound_file()
	{
		assert(audio_buffer != 0);
		alDeleteBuffers(1, &audio_buffer);
	}

	// !! Most of the code here is "borrowed" from the OpenAL examples !!

	bool sound_file::load(const std::string& file_path)
	{
		PROFILER_SCOPE_AUDIO_FN()

		assert(!file_path.empty());

		log("Loading sound file", file_path);

		SNDFILE* sndfile = sf_open(file_path.c_str(), SFM_READ, &sfinfo);

		if (!sndfile)
		{
			log_error("Couldn't open sound file at " + file_path + ". Error: " + sf_strerror(sndfile));
			return false;
		}

		if (sfinfo.frames < 1)
		{
			log_error("Bad sample count error");
			return false;
		}

		// Values for float sampling
		splblockalign = 1;
        byteblockalign = sfinfo.channels * 4;
		format = AL_FORMAT_STEREO_FLOAT32;

		log("Channel count", sfinfo.channels);
		assert(sfinfo.channels == 2 && "Channel count of two required for now");

		if(sfinfo.frames / splblockalign > static_cast<sf_count_t>(INT_MAX / byteblockalign))
		{
			log_error("Too many samples error");
			return false;
		}

		// Allocate a buffer for the sound file
		std::unique_ptr<f32, free_delete> file_buffer(static_cast<f32*>(malloc(sfinfo.frames / splblockalign * byteblockalign)));
        frame_count = sf_readf_float(sndfile, file_buffer.get(), sfinfo.frames);
		assert(alGetError() == AL_NO_ERROR);
		log("Frame count", frame_count);

		if (frame_count < 1)
		{
			log_error("Failed to read samples in");
			return false;
		}

		byte_count = static_cast<ALsizei>(frame_count / splblockalign * byteblockalign);

		// Load the audio into a buffer
		audio_buffer = 0;
		alGenBuffers(1, &audio_buffer);
		log("Block align", splblockalign);
		assert(splblockalign == 1 && "Only block alignment of one is supported at the moment");
		alBufferData(audio_buffer, format, file_buffer.get(), byte_count, sfinfo.samplerate);
		sf_close(sndfile);

		ALenum err = alGetError();
		if(err != AL_NO_ERROR)
		{
			log_error("OpenAL Error: %s\n", alGetString(err));
			if(audio_buffer && alIsBuffer(audio_buffer))
				alDeleteBuffers(1, &audio_buffer);
			return false;
		}

		return true;
	}

	u32 sound_file::buffer()
	{
		return audio_buffer;
	}
}

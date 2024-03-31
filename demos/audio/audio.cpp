#include "AudioDevice.hpp"
#include "AudioPlayer.hpp"
#include "SoundFile.hpp"

#include <iostream>

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: audio [path to file]\n";
		return 1;
	}

	birb::audio_device audio_dev;
	birb::sound_file sound(argv[1]);

	birb::audio_player player;
	player.play_sound(sound);

	std::cout << "Press ENTER to stop playing...";
	std::cin.get();

	return 0;
}

#define CUTE_SOUND_PLATFORM_WINDOWS
#define CUTE_SOUND_IMPLEMENTATION
#include "./include/cute_sound.hpp"
#include <stdio.h>

int main()
{
	HWND hwnd = GetConsoleWindow();
	cs_init(hwnd, 44100, 1024, NULL); // https://github.com/RandyGaul/cute_headers/blob/master/examples_cute_sound/jump.wav
	cs_audio_source_t* jump = cs_load_wav("music/jump.wav", NULL);
	cs_sound_params_t params = cs_sound_params_default();
	cs_playing_sound_t jump_snd = CUTE_PLAYING_SOUND_INVALID; // i told you too smartie
	printf("jump.wav has a sample rate of %d Hz.\n", jump->sample_rate);
// why

	while (1)
	{
			jump_snd = cs_play_sound(jump, params);

		cs_update(0.4f);
	}

	cs_free_audio_source(jump);
}
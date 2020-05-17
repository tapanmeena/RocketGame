#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <bits/stdc++.h>

int main()
{
/*	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	Mix_Music *music;
	music = Mix_LoadMUS("music/backgroundMusic.mp3");
	Mix_PlayMusic(music, 1);*/
	// int result = 0;
	//     int flags = MIX_INIT_MP3;

	//     if (SDL_Init(SDL_INIT_AUDIO) < 0) {
	//         printf("Failed to init SDL\n");
	//         exit(1);
	//     }

	//     if (flags != (result = Mix_Init(flags))) {
	//         printf("Could not initialize mixer (result: %d).\n", result);
	//         printf("Mix_Init: %s\n", Mix_GetError());
	//         exit(1);
	//     }

	//     Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);
	//     Mix_Music *music = Mix_LoadMUS("music/backgroundMusic.mp3");
	//     Mix_PlayMusic(music, 1);
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{
		printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		// success = false;
	}

	Mix_Chunk *gBeep = NULL;
	gBeep = Mix_LoadWAV( "Beep2.wav" );
	if( gBeep == NULL )
	{
		printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
		// success = false;
	}
	Mix_PlayChannel( -1, gBeep, 50);
	std::cout<<"some "<<std::endl;
	return 0;
}
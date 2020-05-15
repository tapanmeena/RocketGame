#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>

#define WINDOW_HEIGHT (1000)
#define WINDOW_WIDTH (1000)
#define SPEED (300)

int main()
{
	int close = 0;
	srand(time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		printf("Error initializing SDL: %s\n", SDL_GetError());

	SDL_Window* win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Render* rend = SDL_CreateRender(win, -1, render_flags);

	SDL_Surface* surface = IMG_LOAD("resources/rocket.png");

	SDL_Texture* rocketTexture = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);

	SDL_Rect rocket, obstacles[10], lives, backgroundObjects[10], backgroundObstacles[10], passengerShip[7], scoreAdder;

	SDL_Texture* bulletsTexture[10], startMenuTexture, exitMenuTexture, obstaclesTexture, gameoverTexture, backgroundObjectsTexture, scoreAdderTexture, passengerShipTexture[7];
	surface = IMG_LOAD("resources/bullet.png");

	for(int i = 0; i<10; i++)
		bulletsTexture[i] = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);

	surface = IMG_LOAD("resources/start.png");

}
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

/*SDL_Texture* TextureFromSurfaceStartMenu(int i, SDL_Renderer* rend)
{
	char integer_string[32];
	sprintf(integer_string, "%d", i);
	char path[64] = "resources/startmenu/";
	strcat(path, integer_string);
	char extension[32] = ".png";
	strcat(path, extension);
	SDL_Surface* surface = IMG_Load(path);
	// SDL_FreeSurface(surface);
	return SDL_CreateTextureFromSurface(rend, surface);
}*/

SDL_Texture* TextureFromSurface(const char* path, SDL_Renderer* rend)
{
	SDL_Surface* surface = IMG_Load(path);
	// SDL_FreeSurface(surface);
	return SDL_CreateTextureFromSurface(rend, surface);
}


int main()
{
	int closeGame = 0;
	srand(time(0));

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		printf("Error initializing SDL: %s\n", SDL_GetError());
	// std::cout<<"1"<<std::endl;
	SDL_Window* win = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);

	Uint32 render_flags = SDL_RENDERER_ACCELERATED;

	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	// std::cout<<"2"<<std::endl;

	// SDL_Surface* surface = IMG_Load("resources/rocket.png");

	SDL_Texture* rocketTex = TextureFromSurface("resources/rocket.png", rend);

	// SDL_FreeSurface(surface);
	// std::cout<<"1"<<std::endl;
	SDL_Rect rocket, obstacles[10], lives, bgObjs[10], bgObst[10], psngrShip[7], scoreAdder, bullets[10], startmenu;
	// std::cout<<"3"<<std::endl;

	SDL_Texture* bltsTex[10], *livesTex, *bgTex, *startMenuTex, *obstaclesTex[10], *gameoverTex, *bgObjTex[10], *scoreAdderTex, *psngrShipTex[7];
	// not using anymone: *exitMenuTex, 
	// surface = IMG_LOAD("resources/bullet.png");

	for(int i = 0; i<10; i++)
		bltsTex[i] = TextureFromSurface("resources/bullet.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/passengerShip.png");
	for (int i = 0; i < 7; i++)
		psngrShipTex[i] = TextureFromSurface("resources/passengerShip.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/start.png");
/*	for(int i = 0; i<26; i++)
		startMenuTex[i] = TextureFromSurfaceStartMenu(i, rend);*/
	startMenuTex = TextureFromSurface("resources/start.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/exit.png");
	// exitMenuTex = TextureFromSurface("resources/exit.png", rend);
	// SDL_FreeSurface(surface);
	
	// surface = IMG_LOAD("resources/obstacle.gif");
	for (int i = 0; i<10; i++)
		obstaclesTex[i] = TextureFromSurface("resources/obstacle.gif", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/gameover.png");
	gameoverTex = TextureFromSurface("resources/gameover.png", rend);
	// SDL_FreeSurface(surface);
	
	// surface = IMG_LOAD("resources/heart.png");
	livesTex = TextureFromSurface("resources/heart.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/backgroundObject.png");
	for (int i = 0; i < 10; ++i)
		bgObjTex[i] = TextureFromSurface("resources/backgroundObject.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/background.png");
	bgTex = TextureFromSurface("resources/background.png", rend);
	// std::cout<<"yaha tak paucha"<<std::endl;
	SDL_QueryTexture(livesTex, NULL, NULL, &lives.w, &lives.h);
	for (int i = 0; i < 10; i++)
		SDL_QueryTexture(bgObjTex[i], NULL, NULL, &bgObjs[i].w, &bgObjs[i].h);
	SDL_QueryTexture(rocketTex, NULL, NULL, &rocket.w, &rocket.h);
	for (int i = 0; i < 10; i++)
		SDL_QueryTexture(obstaclesTex[i], NULL, NULL, &obstacles[i].w, &obstacles[i].h);
	for (int i = 0; i < 10; i++)
		SDL_QueryTexture(bltsTex[i], NULL, NULL, &bullets[i].w, &bullets[i].h);
	// SDL_Rect startmenu;
	SDL_QueryTexture(startMenuTex, NULL, NULL, &startmenu.w, &startmenu.h);
	// std::cout<<" asd "<<startmenu.w/26<<" "<<startmenu.h<<std::endl;
	for (int i = 0; i<7; i++)
	{
		SDL_QueryTexture(psngrShipTex[i], NULL, NULL, &psngrShip[i].w, &psngrShip[i].h);
		// niche ki 2 lines comment karke chalana hai ek baar code
		psngrShip[i].w /= 2;
		psngrShip[i].h /= 2;
	}

	int passengerShipCount[7] = {0, 0, 0, 0, 0, 0, 0};

	// Adjusting Height and Width of Objects
	for (int i=0; i<10; i++)
	{
		bgObjs[i].w /= 4;
		bgObjs[i].h /= 4;
	}

	lives.w /= 4;
	lives.h /= 4;

	rocket.w /= 8;
	rocket.h /= 8;

	for (int i = 0; i<10; i++)
	{
		obstacles[i].w /= 6;
		obstacles[i].h /= 6;
	}

	for(int i = 0; i<10; i++)
	{
		bullets[i].w /= 9;
		bullets[i].h /= 16;
	}

	float rktPos_x = (WINDOW_WIDTH - rocket.w) / 2;
	float rktPos_y = (WINDOW_HEIGHT - rocket.h) / 2;
	float rktVel_x = 0, rktVel_y = 0;
	float bulletPos_x[10], bulletPos_y[10];
	lives.x = WINDOW_WIDTH - lives.w;
	lives.y = 0;
	int gameOption = 1; // 0 for exit 1 for startGame
	while(!closeGame)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					closeGame = 1;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.scancode)
					{
						// case SDL_SCANCODE_UP :
						// 	gameOption = 1;
						// 	break;
						case SDL_SCANCODE_ESCAPE:
							gameOption = 0;
							closeGame = 1;
							break;
						case SDL_SCANCODE_RETURN:
							gameOption = 1;
							closeGame = 1;
							break;
					}
					break;
			}
		}
        SDL_RenderClear(rend);
        // std::cout<<gameOption<<std::endl;
        if (gameOption == 1)
        {
        	Uint32 ticks = SDL_GetTicks();
        	Uint32 sprite = (ticks / 250) % 26;
        	SDL_Rect srcrect = {sprite * 500 , 0, 500 , 275};
        	SDL_Rect dstrect = {112 , 125, 800, 500};
        	SDL_RenderCopy(rend, startMenuTex, &srcrect, &dstrect);
        	// SDL_RenderCopy(rend, startMenuTex[0], NULL, NULL);
/*        	for(int i = 0; i < 26; i++)
        	{
	        	SDL_RenderCopy(rend, startMenuTex[i], NULL, NULL);
        	}*/
        }
        // SDL_RenderClear(rend);
/*        if(gameOption == 0)
        	SDL_RenderCopy(rend, exitMenuTex, NULL, NULL);*/
    	SDL_RenderPresent(rend);
    	SDL_Delay(1000/60);
	}
    SDL_RenderClear(rend);
	SDL_DestroyTexture(startMenuTex);
	std::cout<<"outside\n";

	scoreAdderTex = TextureFromSurface("resources/plus.png", rend);
	SDL_QueryTexture(scoreAdderTex, NULL, NULL, &scoreAdder.w, &scoreAdder.h);

	scoreAdder.w /= 2;
	scoreAdder.h /= 2;
	for(int i = 0; i < 10; i++)
	{
		bgObjs[i].x = i*10;
		bgObjs[i].y = i*10;
	}

	closeGame = 0; //resetting game loop
	int up = 0, down = 0, left = 0, right = 0, shotFired = 0, shots = 0;
	int remainLife = 3;

	while(!closeGame)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					closeGame = 1;
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.scancode)
					{
						case SDL_SCANCODE_UP:
							up = 1;
							break;
						case SDL_SCANCODE_DOWN:
							down = 1;
							break;	
						case SDL_SCANCODE_LEFT:
							left = 1;
							break;
						case SDL_SCANCODE_RIGHT:
							right = 1;
							break;
						case SDL_SCANCODE_SPACE:
							shotFired = 1;
							shots += 1;
							// if (shots > 9)
							break;
						// have to code escape sequnece
						case SDL_SCANCODE_ESCAPE:
							closeGame = 1;
							break;
					}
					break;
				case SDL_KEYUP:
				    switch (event.key.keysym.scancode)
				    {
				        case SDL_SCANCODE_UP:
				            up = 0;
				            break;
				        case SDL_SCANCODE_LEFT:
				            left = 0;
				            break;
				        case SDL_SCANCODE_DOWN:
				            down = 0;
				            break;
				        case SDL_SCANCODE_RIGHT:
			            	right = 0;
				            break;
				    }
					break;
			}
		}

		rktVel_x = 0;
		rktVel_y = 0;

		if (up && !down) rktVel_y = -SPEED;
		if (down && !up) rktVel_y = SPEED;
		if (left && !right) rktVel_x = -SPEED;
		if (right && !left) rktVel_x = SPEED;

		rktPos_x += rktVel_x / 30;
		rktPos_y += rktVel_y / 30;





		SDL_RenderClear(rend);
		SDL_RenderCopy(rend, bgTex, NULL, NULL);
		SDL_RenderCopy(rend, livesTex, NULL, &lives);
		SDL_RenderPresent(rend);
		SDL_RenderCopy(rend, rocketTex, NULL, &rocket);
		SDL_RenderPresent(rend);

		SDL_Delay(1000/30);
	}

	// destroy renderer 
	SDL_DestroyRenderer(rend); 

	// destroy window 
	SDL_DestroyWindow(win); 

	return 0;
}

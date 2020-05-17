#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <bits/stdc++.h>

const int WINDOW_HEIGHT = 1000;
const int WINDOW_WIDTH = 1000;
const int SPEED = 300;
const int difficulty = 1;
const int numObstacles = 4;
const int numBullets = 10;
const int numPsngrShip = 7;
const int frameRate = 1000/30;

bool isCollide(SDL_Rect object1, SDL_Rect object2)
{
	if((object1.y >= object2.y) && (object1.y <= (object2.y + object2.h)) && (object1.x >= object2.x) && (object1.x <= (object2.x + object2.w)))
		return true;
	return false;
}

bool isDestroyed(SDL_Rect object1, SDL_Rect object2)
{
	if(((object1.y + object1.h) >= object2.y) && ((object1.y + object1.h) <= (object2.y + object2.h)) && (object1.x + object1.w >= object2.x) && (object1.x <= (object2.x + object2.w)))
		return true;
	return false; 
}

bool isRocketHit(SDL_Rect object1, SDL_Rect object2)
{
	if ((object1.y + object1. h) >= object2.y && ((object2.y + object2.h - 10) >= object1.y) && (object2.x <= (object1.x +object1.w)) && ((object2.x + object2.w) >= object1.x))
		return true;
	return false;
}

SDL_Texture* TextureFromSurface(const char* path, SDL_Renderer* rend)
{
	SDL_Surface* surface = IMG_Load(path);
	// SDL_FreeSurface(surface);
	return SDL_CreateTextureFromSurface(rend, surface);
}


int main()
{
/*	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
		printf("%s", Mix_GetError());
	Mix_Music *music;
	music = Mix_LoadMUS("music/backgroundMusic.mp3");
	Mix_PlayMusic(music, -1);*/
	int closeGame = 0;
	srand(time(0));
	time_t currentTime;
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
	SDL_Rect rocket, obstacles[10], lives, bgObjs[10], bgObst[10], psngrShip[7], scoreAdder, bullets[10], startmenu, explode;
	// std::cout<<"3"<<std::endl;

	SDL_Texture* bltsTex[10], *livesTex, *bgTex, *startMenuTex, *obstaclesTex[10], *gameoverTex, *bgObjTex[10], *scoreAdderTex, *psngrShipTex[7], *explodeTex;
	// not using anymone: *exitMenuTex, 
	// surface = IMG_LOAD("resources/bullet.png");
// restart:
	for(int i = 0; i<numBullets; i++)
		bltsTex[i] = TextureFromSurface("resources/bullet.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/passengerShip.png");
	for (int i = 0; i < numPsngrShip; i++)
		psngrShipTex[i] = TextureFromSurface("resources/passengerShip.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/start.png");
	/*for(int i = 0; i<26; i++)
		startMenuTex[i] = TextureFromSurfaceStartMenu(i, rend);*/
	startMenuTex = TextureFromSurface("resources/start.png", rend);
	// SDL_FreeSurface(surface);

	// surface = IMG_LOAD("resources/exit.png");
	// exitMenuTex = TextureFromSurface("resources/exit.png", rend);
	// SDL_FreeSurface(surface);
	
	// surface = IMG_LOAD("resources/obstacle.gif");
	for (int i = 0; i<numObstacles; i++)
		obstaclesTex[i] = TextureFromSurface("resources/obstacle.png", rend);
	// SDL_FreeSurface(surface);
	explodeTex = TextureFromSurface("resources/explode.png", rend);

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
	for (int i = 0; i < numObstacles; i++)
		SDL_QueryTexture(obstaclesTex[i], NULL, NULL, &obstacles[i].w, &obstacles[i].h);
	for (int i = 0; i < numBullets; i++)
		SDL_QueryTexture(bltsTex[i], NULL, NULL, &bullets[i].w, &bullets[i].h);
	// SDL_Rect startmenu;
	SDL_QueryTexture(startMenuTex, NULL, NULL, &startmenu.w, &startmenu.h);
	SDL_QueryTexture(explodeTex, NULL, NULL, &explode.w, &explode.h);
	// std::cout<<explode.w<<" "<<explode.h<<std::endl;
	// std::cout<<" asd "<<startmenu.w/26<<" "<<startmenu.h<<std::endl;
	for (int i = 0; i<numPsngrShip; i++)
	{
		SDL_QueryTexture(psngrShipTex[i], NULL, NULL, &psngrShip[i].w, &psngrShip[i].h);
		// niche ki 2 lines comment karke chalana hai ek baar code
		psngrShip[i].w /= 2;
		psngrShip[i].h /= 2;
	}

	int passengerShipCount[numPsngrShip] = {0, 0, 0, 0, 0, 0, 0};

	// Adjusting Height and Width of Objects
	for (int i=0; i < 10; i++)
	{
		bgObjs[i].w /= 4;
		bgObjs[i].h /= 4;
	}

	lives.w /= 4;
	lives.h /= 4;

	rocket.w /= 8;
	rocket.h /= 8;

	explode.w /= 8;
	explode.h /= 8;

	for (int i = 0; i < numObstacles; i++)
	{
		obstacles[i].w /= 6;
		obstacles[i].h /= 6;
	}

	for(int i = 0; i < numBullets; i++)
	{
		bullets[i].w /= 6;
		bullets[i].h /= 6;
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
        	int sprite = (ticks / 250) % 26;
        	SDL_Rect srcrect = {sprite * 500 , 0, 500 , 275};
        	SDL_Rect dstrect = {112 , 125, 800, 500};
        	SDL_RenderCopy(rend, startMenuTex, &srcrect, &dstrect);
        	// SDL_RenderCopy(rend, startMenuTex[0], NULL, NULL);
        	/*for(int i = 0; i < 26; i++)
        	{
	        	SDL_RenderCopy(rend, startMenuTex[i], NULL, NULL);
        	}*/
        }
        // SDL_RenderClear(rend);
        /*if(gameOption == 0)
        	SDL_RenderCopy(rend, exitMenuTex, NULL, NULL);*/
    	SDL_RenderPresent(rend);
    	SDL_Delay(1000/60);
	}
    SDL_RenderClear(rend);
	SDL_DestroyTexture(startMenuTex);
	// std::cout<<"outside\n";

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
	int obsInitilizer[10];
	memset(obsInitilizer, 0, sizeof(obsInitilizer));
	int shotsAlive[10];
	memset(shotsAlive, 0, sizeof(shotsAlive));
	std::queue<int> bulletsQueue;
	for(int i = 0; i<10; i++)
		bulletsQueue.push(i);
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

		// from this rocket can move in 8 dimensions
		if (up && !down) rktVel_y = -SPEED;
		if (down && !up) rktVel_y = SPEED;
		if (left && !right) rktVel_x = -SPEED;
		if (right && !left) rktVel_x = SPEED;

		rktPos_x += rktVel_x / 30;
		rktPos_y += rktVel_y / 30;

		// creating rocket boundries
		if (rktPos_x <= 0)
			rktPos_x = WINDOW_WIDTH - rocket.w;
		else if(rktPos_x >= WINDOW_WIDTH - rocket.w)
			rktPos_x = 0;

		if (rktPos_y <= 100)
			rktPos_y = 100;
		else if (rktPos_y >= WINDOW_HEIGHT - rocket.h)
			rktPos_y = WINDOW_HEIGHT - rocket.h;

		for (int i = 0; i < numPsngrShip; i++)
		{
			psngrShip[i].x = 50 + psngrShip[i].w * i;
			psngrShip[i].y = WINDOW_HEIGHT - psngrShip[i].h;
		}

		rocket.y = (int) rktPos_y;
		rocket.x = (int) rktPos_x;

		for(int i = 0; i < numObstacles; i++)
		{
			if(obsInitilizer[i] == 0)
			{
				obstacles[i].x = rand() % 800;
				obsInitilizer[i] = 1;
				obstacles[i].y = 0;
			}
			else
				obstacles[i].y = obstacles[i].y + (SPEED/50) * difficulty;
		}

		if (shotFired)
		{
			int frontBullet = bulletsQueue.front();
			bulletsQueue.pop();
			bulletsQueue.push(frontBullet);
			bulletPos_x[frontBullet] = rocket.x + (rocket.w/2) - (bullets[0].w/2);
			bulletPos_y[frontBullet] = rocket.y;
			shotFired = 0;
		}

		for (int i = 0; i < numBullets; i++)
		{
			bullets[i].x = bulletPos_x[i];
			bullets[i].y = bulletPos_y[i];
			bulletPos_y[i] = bulletPos_y[i] - SPEED / 50; //can be change later for speeding Bullet
		}
		SDL_RenderClear(rend);

		SDL_RenderCopy(rend, bgTex, NULL, NULL);
		// SDL_RenderPresent(rend);
		SDL_RenderCopy(rend, rocketTex, NULL, &rocket);
		// SDL_RenderPresent(rend);
		SDL_RenderCopy(rend, livesTex, NULL, &lives);
		SDL_RenderPresent(rend);

		for(int i=0;i < numBullets;i++)				// shows bullets on the screen
		    SDL_RenderCopy(rend, bltsTex[i], NULL, &bullets[i]);
		SDL_RenderPresent(rend);

		// render passenger ships
		for (int i = 0; i < numPsngrShip; i++)
		{
			if (passengerShipCount[i] == 0)
				SDL_RenderCopy(rend, psngrShipTex[i], NULL, &psngrShip[i]);
		}
		SDL_RenderPresent(rend);

		// render obstacles
		for (int i = 0; i < numObstacles; i++)
			SDL_RenderCopy(rend, obstaclesTex[i], NULL, &obstacles[i]);

		//------------- all things rendered now ------------ except no. of hearts currently using sprite//

		// ----------------- now doing collision checking ------------------//
		// bool collide = false;
		//checking collision between bullets and asteriods
		for(int i = 0; i < numBullets; i++)
		{
			for(int j = 0; j < numObstacles; j++)
			{
				if (isCollide(bullets[i], obstacles[j]))
				{
					obsInitilizer[j] = 0;
					scoreAdder.x = rocket.x;
					scoreAdder.y = rocket.y - 30;
					explode.x = obstacles[j].x;
					explode.y = obstacles[j].y;
					currentTime = time(NULL);
				}
			}
		}

		// for explosion and scoreadder
		time_t tempTime = time(NULL);
		if(tempTime < currentTime+1.5)
		{
			SDL_RenderCopy(rend, scoreAdderTex, NULL, &scoreAdder);
			SDL_RenderCopy(rend, explodeTex, NULL, &explode);			
		}

		// checking collision between asteriod and passenger ship
		for(int i = 0; i<numObstacles; i++)
		{
			for(int j = 0; j < numPsngrShip; j++)
			{
				if(passengerShipCount[j] == 0)
				{
					if(isDestroyed(obstacles[i], psngrShip[j]))
					{
						// std::cout<<"Ship destroyed\n";
						passengerShipCount[j] = 1;
						SDL_DestroyTexture(psngrShipTex[j]);
					}
				}
			}
		}

		// checking collision between asteriod and Rocket Ship
		for (int i = 0; i<numObstacles; i++)
		{
			if(isRocketHit(obstacles[i], rocket))
			{
				obsInitilizer[i] = 0;
				remainLife --;
				if (remainLife < 0)
					closeGame = 1;
				lives.x += lives.w/3;
				// SDL_Delay(1000/30);
			}
		}

		// respawning destroyed Obstacles
		for(int i = 0; i < numObstacles; i++)
			if (obstacles[i].y >= WINDOW_HEIGHT || obstacles[i].x < 0)
				obsInitilizer[i] = 0;

		// checking for Game Over
		int count = 0;
		for (int i = 0; i < numPsngrShip; i++)
			if (passengerShipCount[i] == 0)
				count++;
		if (count == 0)
			closeGame = 1;

		SDL_RenderPresent(rend);
		SDL_Delay(frameRate);
	}

	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, gameoverTex, NULL, NULL);
	SDL_RenderPresent(rend);
	SDL_Delay(3000);
	// goto restart;

	SDL_DestroyTexture(startMenuTex);
	SDL_DestroyTexture(livesTex);
	SDL_DestroyTexture(bgTex);
	SDL_DestroyTexture(scoreAdderTex);
	SDL_DestroyTexture(explodeTex);
	for (int i = 0; i < 10; ++i)
	{
		SDL_DestroyTexture(bltsTex[i]);
		SDL_DestroyTexture(obstaclesTex[i]);
		if(i<7)
			SDL_DestroyTexture(psngrShipTex[i]);
	}
	SDL_DestroyRenderer(rend); 
	SDL_DestroyWindow(win); 
	// SDL_QUIT();
	return 0;
}
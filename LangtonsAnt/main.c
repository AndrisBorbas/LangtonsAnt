// Copyright 2018 Andirs Borbás, All Rights Reserved.

#include "main.h"

int main(int argc, char ** argv)
{
	SDL_Color TextDARKWHITE = { 221,221,221 }, TextORANGE = { 247,99,12 };

	char filename[52] = "config.ini";

	if (argc > 1)strcpy(filename, argv[1]);

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;

	//The config file
	FILE *wDefConf;
	wDefConf = fopen(filename, "r+");
	if (wDefConf == NULL)
	{
		printf("Could not open config file.");
		exit(30);
	}

	//Buffer for reading from file
	char buffer[52] = "";

	////////////////
	//Default values
	////////////////

	//Window parameter container
	volatile SDL_Rect SCREEN;
	//Width of the window
	SCREEN.w = 960;
	//Height of the window
	SCREEN.h = 960;
	//Number of pixels per grid square (has to be at least 3 or the ant wont be visible, but it will still work)
	volatile int SCALE = 4;
	//Number of pixels between grid squares
	volatile int SPACING = 1;
	//Number of pixels the ant is smaller than the grid (has to be at least 1, can't be more than SCALE)
	int const ANTMARGIN = 1;
	//The tickrate of the simulation in ms
	volatile int MSTICK = 16;
	//Instructionset for the ant
	volatile char instructionset[19] = "LR";

	//Number of steps made by the ant;
	int lepes = 0;

	//Load values from config file
	while ((strstr(buffer, "endconfig;")) == NULL)
	{
		fscanf(wDefConf, "%s", &buffer);
		if (buffer[0] == '/')continue;

		if (buffer[0] == '#')
		{
			loadintFromConfig(wDefConf, buffer, &SCREEN.w, "SCREEN_WIDTH");
			loadintFromConfig(wDefConf, buffer, &SCREEN.h, "SCREEN_HEIGHT");
			loadintFromConfig(wDefConf, buffer, &SCALE, "SCALE");
			loadintFromConfig(wDefConf, buffer, &SPACING, "SPACING");
			loadintFromConfig(wDefConf, buffer, &ANTMARGIN, "ANTMARGIN");
			loadintFromConfig(wDefConf, buffer, &MSTICK, "MSTICK");
			loadcharFromConfig(wDefConf, buffer, &instructionset, "INSTRUCTIONSET");
		}
	}
	int instructnum = strlen(instructionset);

	//Buttons
	SDL_Rect StartButton;
	StartButton.w = 252;
	StartButton.h = 80;
	StartButton.x = SCREEN.w / 2 - StartButton.w / 2;
	StartButton.y = 25;

	SDL_Rect StartButtonStroke;
	int Strokesize = 2;
	StartButtonStroke.w = StartButton.w + (Strokesize * 2);
	StartButtonStroke.h = StartButton.h + (Strokesize * 2);
	StartButtonStroke.x = StartButton.x - Strokesize;
	StartButtonStroke.y = StartButton.y - Strokesize;

	SDL_Rect ResButton;
	if (SCREEN.w >= 1000)ResButton.w = 160;
	else ResButton.w = 144;
	ResButton.h = 40;
	ResButton.x = SCREEN.w / 2 + SCREEN.w / 4 - ResButton.w / 2;
	ResButton.y = SCREEN.h - ResButton.h - 25;

	SDL_Rect ResUp;
	ResUp.w = 34;
	ResUp.h = 34;
	ResUp.x = ResButton.x + ResButton.w + 8;
	ResUp.y = ResButton.y + 3;

	SDL_Rect ResDown;
	ResDown.w = 34;
	ResDown.h = 34;
	ResDown.x = ResButton.x - ResDown.w - 8;
	ResDown.y = ResButton.y + 3;



	//Handle normal quit
	bool quit = false;
	//Handle quit on error
	bool equit = false;
	//Start the algorithm
	bool start = false;
	//Let the algorithm run
	bool Running = true;
	//Is there an error
	bool ERROR = false;

	//The viewport window
	SDL_Window *gWindow = NULL;

	//The renderer
	SDL_Renderer *gRenderer = NULL;

	//The pixel arrays
	Uint32 *pixels = NULL;
	Uint32 **pixelTex = NULL;

	//Textues
	SDL_Texture *tPixelTexture = NULL;
	SDL_Texture *tMainMenu = NULL;

	//Strings
	SDL_Texture *tStrings = NULL;
	SDL_Surface *sStrings = NULL;
	SDL_Rect lStrings = {1,0,0,0};

	//Mouse location
	SDL_Point mouse = { 0,0 };

	//Start up SDL and create window
	if (!initSDL(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN))
	{
		printf("Failed to initialize: %s", SDL_GetError());
		exit(1);
	}

	//Load StartFont
	TTF_Font *StartFont = TTF_OpenFont("ExodusDemo-Striped.otf", 60);
	if (!StartFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(11);
	}

	//Load StartFont
	TTF_Font *MenuFont = TTF_OpenFont("ExodusDemo-Striped.otf", 30);
	if (!MenuFont)
	{
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(12);
	}

	//Load main menu background
	tMainMenu = IMG_LoadTexture(gRenderer, "MainMenu.png");
	if (!tMainMenu)
	{
		SDL_Log("Failed to open image: %s", IMG_GetError());
		exit(21);
	}

	//Draw main menu
	drawMenu(&sStrings, &StartFont, &MenuFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown);

	SDL_Event event;

	while (!quit && !start)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		/*case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
			}
			break;*/
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
				if (SDL_PointInRect(&mouse, &StartButton))
				{
					start = true;
				}

				if (SDL_PointInRect(&mouse, &ResButton))
				{
					refreshMenu(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN, Strokesize, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown);
					drawMenu(&sStrings, &StartFont, &MenuFont, &tStrings, &lStrings, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN, &StartButton, &StartButtonStroke, &ResButton, &ResUp, &ResDown);
				}

				if (SDL_PointInRect(&mouse, &ResUp))
				{
					switch (SCREEN.w)
					{
					case 500:
						SCREEN.w = 720;
						SCREEN.h = 720;
						break;
					case 720:
						SCREEN.w = 900;
						SCREEN.h = 900;
						break;
					case 900:
						SCREEN.w = 960;
						SCREEN.h = 960;
						break;
					case 960:
						SCREEN.w = 1000;
						SCREEN.h = 1000;
						break;
					case 1000:
						SCREEN.w = 1000;
						SCREEN.h = 1000;
						break;
					default:
						SCREEN.w = 960;
						SCREEN.h = 960;
						break;
					}
#ifdef DEBUG
					printf("%d\n", SCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 4 + 2];
					snprintf(buff, sizeof buff, "%dx%d", SCREEN.w, SCREEN.h);
					drawTextintoButton(gRenderer, &sStrings, MenuFont, &tStrings, &lStrings, ResButton, buff, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}

				if (SDL_PointInRect(&mouse, &ResDown))
				{
					switch (SCREEN.h)
					{
					case 1000:
						SCREEN.w = 960;
						SCREEN.h = 960;
						break;
					case 960:
						SCREEN.w = 900;
						SCREEN.h = 900;
						break;
					case 900:
						SCREEN.w = 720;
						SCREEN.h = 720;
						break;
					case 720:
						SCREEN.w = 500;
						SCREEN.h = 500;
						break;
					case 500:
						SCREEN.w = 500;
						SCREEN.h = 500;
						break;
					default:
						SCREEN.w = 960;
						SCREEN.h = 960;
						break;
					}
#ifdef DEBUG
					printf("%d\n", SCREEN.w);
#endif
					roundedBoxColor(gRenderer, ResButton.x, ResButton.y, ResButton.x + ResButton.w, ResButton.y + ResButton.h, 6, altGRAY);
					char buff[sizeof(int) * 4 + 2];
					snprintf(buff, sizeof buff, "%dx%d", SCREEN.w, SCREEN.h);
					drawTextintoButton(gRenderer, &sStrings, MenuFont, &tStrings, &lStrings, ResButton, buff, TextORANGE);
					SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
					SDL_RenderPresent(gRenderer);
				}
			}
			break;
		}
	}

	//The ant
	Ant ant = { SCREEN.w / 2 , SCREEN.w / 2 , LEFT, 0, 0, BLACK };
	for (int i = 0; i < 19; i++) 
	{
		if (instructionset[i] == '\0')break;
		switch (instructionset[i])
		{
		case 'R':
		case 'r':
			ant.turn[i] = 90;
			break;
		case 'L':
		case 'l':
			ant.turn[i] = -90;
			break;
		case 'N':
		case 'n':
			ant.turn[i] = 0;
			break;
		case 'U':
		case 'u':
			ant.turn[i] = 180;
			break;
		}
	}

	//Initialize pixel texture
	initTexture(&gRenderer, &tPixelTexture, SCREEN);
	//Initialize pixel arrays
	initPixels(&pixels, &pixelTex, SCREEN);

	//Create base tickrate
	SDL_TimerID tick = SDL_AddTimer(MSTICK, ftick, NULL);

	SDL_RenderClear(gRenderer);

	while (!quit)
	{
		/*
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();

		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		printf("\n%g\n\n", deltaTime);*/

		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_g:
				for (int i = 0; i < 104; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_h:
				for (int i = 0; i < 1040; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_j:
				for (int i = 0; i < 10400; i++)
				{
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				break;
			case SDLK_SPACE:
				Running = true;
				break;
			}
		case SDL_USEREVENT:
			while (Running && !quit)
			{
				if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
				{
					printf("Error while trying to move ant: %s", SDL_GetError());
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex, SCREEN);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN.w * sizeof(Uint32));
				if (!ERROR) 
				{
					SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
					SDL_RenderPresent(gRenderer);
				}
				SDL_WaitEvent(&event);
				switch (event.type)
				{
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_SPACE:
						Running = false;
						break;
					}
				}
			}
			break;
		}
		if (ERROR) 
		{
			while (!equit)
			{
				SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
				SDL_RenderPresent(gRenderer);

				SDL_WaitEvent(&event);
				if (event.type == SDL_QUIT)
				{
					equit = true;
				}
			}
		}
		else
		{
			SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}
	
#ifdef DEBUG
	debugmalloc_dump();
#endif

	//Free resources and close SDL
	close(&pixels, &pixelTex, &gWindow, gRenderer, tPixelTexture, tMainMenu);
	return 0;
}

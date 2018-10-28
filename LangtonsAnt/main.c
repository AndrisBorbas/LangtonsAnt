#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include "everything.h"

int main(int argc, char ** argv)
{
	//The config file
	FILE *wDefConf;
	wDefConf = fopen("default.cfg", "r+");

	char buffer[52] = "";

	//Width of the window
	volatile int SCREEN_WIDTH = 960;
	//Height of the window
	volatile int SCREEN_HEIGHT = 960;
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



	while ((strstr(buffer, "endconfig;")) == NULL)
	{
		fscanf(wDefConf, "%s", &buffer);
		if (buffer[0] == '/')continue;

		if (buffer[0] == '#') {
			loadintFromConfig(wDefConf, buffer, &SCREEN_WIDTH, "SCREEN_WIDTH");
			loadintFromConfig(wDefConf, buffer, &SCREEN_HEIGHT, "SCREEN_HEIGHT");
			loadintFromConfig(wDefConf, buffer, &SCALE, "SCALE");
			loadintFromConfig(wDefConf, buffer, &SPACING, "SPACING");
			loadintFromConfig(wDefConf, buffer, &ANTMARGIN, "ANTMARGIN");
			loadintFromConfig(wDefConf, buffer, &MSTICK, "MSTICK");
			loadcharFromConfig(wDefConf, buffer, &instructionset, "INSTRUCTIONSET");
		}
	}
	int instructnum = strlen(instructionset);

	//The ant
	Ant ant = { SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 , LEFT, 0, 0, BLACK };
	for (int i = 0; i < 19; i++) {
		if (instructionset[i] == '\0')break;
		switch (instructionset[i]) {
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

	//Click to start
	SDL_Rect startbutton;
	startbutton.w = 150;
	startbutton.h = 40;
	startbutton.x = SCREEN_WIDTH / 2 - startbutton.w / 2;
	startbutton.y = 100;

	SDL_Color sdlaltDARKWHITE = { 221,221,221 }, sdlaltORANGE = { 247,99,12 };

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

	//The window we'll be rendering to
	SDL_Window *gWindow = NULL;

	//The pixel renderer
	SDL_Renderer *gRenderer = NULL;

	Uint32 *pixels = NULL;
	Uint32 **pixelTex = NULL;

	SDL_Texture *tPixelTexture = NULL;
	SDL_Texture *tMainMenu = NULL;
	SDL_Texture *tStrings = NULL;
	SDL_Surface *sStrings = NULL;
	SDL_Rect lStrings = { 0,0,0,0 };

	SDL_Point mouse = { 0,0 };

	//Start up SDL and create window
	if (!initSDL(&gWindow, &gRenderer, &tPixelTexture, &tMainMenu, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		printf("Failed to initialize: %s", SDL_GetError());
		exit(1);
	}

	//Load font
	TTF_Font *font = TTF_OpenFont("ExodusDemo-Striped.otf", 32);
	if (!font) {
		SDL_Log("Failed to open font: %s", TTF_GetError());
		exit(1);
	}

	//Load main menu background
	tMainMenu = IMG_LoadTexture(gRenderer, "MainMenu.png");
	if (tMainMenu == NULL)
	{
		SDL_Log("Failed to open image: %s", IMG_GetError());
		exit(1);
	}

	//Render main menu
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, tMainMenu, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	roundedBoxColor(gRenderer, startbutton.x, startbutton.y, startbutton.x + startbutton.w, startbutton.y + startbutton.h, 6, altGRAY);
	drawText(gRenderer, &sStrings, font, &tStrings, &lStrings, startbutton, "Start", sdlaltORANGE);
	SDL_RenderCopy(gRenderer, tStrings, NULL, &lStrings);
	SDL_RenderPresent(gRenderer);

	SDL_Event event;

	while (!quit && !start)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState(&mouse.x, &mouse.y);
				if (SDL_PointInRect(&mouse, &startbutton)) start = true;
				//if (SDL_PointInRect(&mouse, &startbutton)) ;
			}
			break;
		}
	}


	//Initialize pixel textures
	initPixels(&pixels, &pixelTex, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Create base tickrate
	SDL_TimerID tick = SDL_AddTimer(MSTICK, ftick, NULL);



	while (!quit)
	{
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
				for (int i = 0; i < 104; i++) {
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN_WIDTH, SCREEN_HEIGHT);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				break;
			case SDLK_h:
				for (int i = 0; i < 1040; i++) {
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN_WIDTH, SCREEN_HEIGHT);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				break;
			case SDLK_j:
				for (int i = 0; i < 10400; i++) {
					if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
					{
						ERROR = true;
						quit = true;
						break;
					}
				}
				convertPixels(&pixels, &pixelTex, SCREEN_WIDTH, SCREEN_HEIGHT);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				break;
			case SDLK_SPACE:
				Running = true;
				break;
			}
		case SDL_USEREVENT:
			while (Running && !quit)
			{
				if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum, instructionset))
				{
					printf("Error while trying to move ant: %s", SDL_GetError());
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex, SCREEN_WIDTH, SCREEN_HEIGHT);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				if (!ERROR) {
					//SDL_RenderClear(gRenderer);
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
		if (ERROR) {
			while (!equit)
			{
				SDL_WaitEvent(&event);
				if (event.type == SDL_QUIT)
				{
					equit = true;
				}
			}
		}
		else
		{
			//SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}
	//Free resources and close SDL
	close(&pixels, &pixelTex, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu);
	return 0;
}
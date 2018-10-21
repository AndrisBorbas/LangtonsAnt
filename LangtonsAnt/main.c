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
	//Width of the window
	volatile int SCREEN_WIDTH = 960;
	//Height of the window
	volatile int SCREEN_HEIGHT = 960;
	//Number of pixels per grid square (has to be at least 3 or the ant wont be visible, but it will still work)
	volatile int SCALE = 1;
	//Number of pixels between grid squares
	volatile int SPACING = 0;
	//Number of pixels the ant is smaller than the grid (has to be at least 1)
	volatile int ANTMARGIN = 1;
	//The tickrate of the simulation in ms
	volatile int mstick = 16;

	volatile char* instructions = malloc(5*sizeof(char));


	int instructnum = strlen(instructions);
	//Number of steps made by the ant;
	int lepes = 0;

	Ant ant = { SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 , LEFT, DARKWHITE, BLACK };

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

	bool leftMouseButtonDown = false;

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
	SDL_TimerID tick = SDL_AddTimer(mstick, ftick, NULL);



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
				if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum))
				{
					ERROR = true;
					quit = true;
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
				if (!moveAnt(&pixelTex, &ant, &lepes, SCREEN_WIDTH, SCREEN_HEIGHT, SCALE, SPACING, ANTMARGIN, instructnum))
				{
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
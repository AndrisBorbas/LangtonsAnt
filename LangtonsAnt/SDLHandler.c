#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "everything.h"

bool initSDL(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, int const SCREEN_WIDTH, int const SCREEN_HEIGHT)
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Create window
		*gWindow = SDL_CreateWindow("Langton's Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (*gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			//gScreenSurface = SDL_GetWindowSurface(gWindow);

			//Initialize the renderer
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, 0);

			//Initialize screen texture
			*tPixelTexture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
			*tMainMenu = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
			TTF_Init();
		}
	}
#ifdef __Win32__
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif
	return success;
}

bool initPixels(Uint32** pixels, Uint32*** pixelTex, int const SCREEN_WIDTH, int const SCREEN_HEIGHT) {
	//Initialize pixel array
	*pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(**pixels));

	//Initialize 2D pixel texture array
	*pixelTex = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN_WIDTH);
	*pixelTex[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
		(*pixelTex)[i] = (**pixelTex + SCREEN_HEIGHT * i);

	//Override all pixels
	memset32(*pixels, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset32(**pixelTex, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	return true;
}

void close(Uint32** pixels, Uint32*** pixelTex, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu)
{
	//Free up allocated pixel array
	free(*pixels);
	free(*pixelTex);

	//Destroy texture
	SDL_DestroyTexture(*tPixelTexture);
	SDL_DestroyTexture(*tMainMenu);
	*tMainMenu = NULL;
	*tPixelTexture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(*gRenderer);
	*gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(*gWindow);
	*gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}
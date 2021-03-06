﻿#include "SDLHandler.h"

bool initSDL(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu, SDL_Rect SCREEN)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Create window
		*gWindow = SDL_CreateWindow("Langton's Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN.w, SCREEN.h, SDL_WINDOW_SHOWN);
		if (!*gWindow)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			//Initialize the renderer
			*gRenderer = SDL_CreateRenderer(*gWindow, -1, 0);

			//initTexture(gRenderer, tPixelTexture, SCREEN);
			initTexture(gRenderer, tMainMenu, SCREEN);

			//Initialize the font engine
			TTF_Init();
		}
	}
#ifdef __Win32__
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif
	return true;
}

void initTexture(SDL_Renderer** gRenderer, SDL_Texture** tTexture, SDL_Rect SCREEN)
{
	if (*tTexture != NULL)SDL_DestroyTexture(*tTexture);
	*tTexture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN.w, SCREEN.h);
}

void initPixels(Uint32** pixels, Uint32*** pixelTex, SDL_Rect SCREEN)
{
	//Free if they are already allocated
	if (*pixels != NULL)free(*pixels);
	if (*pixelTex != NULL)
	{
		free(*pixelTex[0]);
		free(*pixelTex);
	}

	//Initialize pixel array
	*pixels = malloc(SCREEN.w * SCREEN.h * sizeof(**pixels));

	//Initialize 2D pixel texture array
	*pixelTex = malloc(sizeof(Uint32 *) * SCREEN.w);
	*pixelTex[0] = malloc(sizeof(Uint32) * SCREEN.h * SCREEN.w);
	for (int i = 0; i < SCREEN.w; i++)
		(*pixelTex)[i] = (**pixelTex + SCREEN.h * i);

	//Override all pixels
	memset32(*pixels, BLACK, SCREEN.w * SCREEN.h * sizeof(Uint32));
	memset32(*pixelTex[0], BLACK, SCREEN.w * SCREEN.h * sizeof(Uint32));
}

void close(Uint32** pixels, Uint32*** pixelTex, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* tPixelTexture, SDL_Texture* tMainMenu, SDL_Texture* tStrings)
{
	//Free up allocated pixel arrays
	if (*pixels != NULL)free(*pixels);
	if (*pixelTex != NULL)
	{
		free(*pixelTex[0]);
		free(*pixelTex);
	}

	//Destroy textures
	SDL_DestroyTexture(tStrings);
	SDL_DestroyTexture(tPixelTexture);
	SDL_DestroyTexture(tMainMenu);
	tMainMenu = NULL;
	tPixelTexture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

////////////////////////////////////////////////////////////////
//Taken from an online forum to make the program generate images
//https://stackoverflow.com/questions/34255820/save-sdl-texture-to-file
///////////////////////////////////////////////////////////////////////

void save_texture(SDL_Renderer* gRenderer, SDL_Texture* tTexture, const char *filename)
{
	SDL_Texture *tRender;
	SDL_Surface *surface;
	int st;
	int w;
	int h;
	int format;
	void *pixels;

	pixels = NULL;
	surface = NULL;
	tRender = NULL;
	format = SDL_PIXELFORMAT_RGBA32;

	/* Get information about texture we want to save */
	st = SDL_QueryTexture(tTexture, NULL, NULL, &w, &h);
	if (st != 0) {
		SDL_Log("Failed querying texture: %s\n", SDL_GetError());
		goto cleanup;
	}

	tRender = SDL_CreateTexture(gRenderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
	if (!tRender) {
		SDL_Log("Failed creating render texture: %s\n", SDL_GetError());
		goto cleanup;
	}

	/*
	 * Initialize our canvas, then copy texture to a target whose pixel data we
	 * can access
	 */
	st = SDL_SetRenderTarget(gRenderer, tRender);
	if (st != 0) {
		SDL_Log("Failed setting render target: %s\n", SDL_GetError());
		goto cleanup;
	}

	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	st = SDL_RenderCopy(gRenderer, tTexture, NULL, NULL);
	if (st != 0) {
		SDL_Log("Failed copying texture data: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Create buffer to hold texture data and load it */
	pixels = malloc(w * h * SDL_BYTESPERPIXEL(format));
	if (!pixels) {
		SDL_Log("Failed allocating memory\n");
		goto cleanup;
	}

	st = SDL_RenderReadPixels(gRenderer, NULL, format, pixels, w * SDL_BYTESPERPIXEL(format));
	if (st != 0) {
		SDL_Log("Failed reading pixel data: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Copy pixel data over to surface */
	surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, SDL_BITSPERPIXEL(format), w * SDL_BYTESPERPIXEL(format), format);
	if (!surface) {
		SDL_Log("Failed creating new surface: %s\n", SDL_GetError());
		goto cleanup;
	}

	/* Save result to an image */
	st = SDL_SaveBMP(surface, filename);
	if (st != 0) {
		SDL_Log("Failed saving image: %s\n", SDL_GetError());
		goto cleanup;
	}

	SDL_Log("Saved texture as BMP to \"%s\"\n", filename);

cleanup:
	SDL_FreeSurface(surface);
	free(pixels);
	SDL_DestroyTexture(tRender);
}
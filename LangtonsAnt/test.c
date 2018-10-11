#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

//Init SDL
bool sdl_init(Uint32** gPixels, Uint32*** gPixelArr2D, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture, int SCREEN_WIDTH, int SCREEN_HEIGHT)
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
			*gTexture = SDL_CreateTexture(*gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

			//Initialize pixel array
			*gPixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(**gPixels));
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*gPixels[500 * SCREEN_WIDTH + 500] = 0xFFFF0000;

			//Initialize 2D pixel texture array
			*gPixelArr2D = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN_WIDTH);
			*gPixelArr2D[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH);
			for (int i = 0; i < SCREEN_WIDTH; i++)
				(*gPixelArr2D)[i] = (**gPixelArr2D + SCREEN_HEIGHT * i);

			//Override all pixels to white
			memset(*gPixels, 255255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
			memset(**gPixelArr2D, 255255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		}
	}

	return success;
}

void sdl_close(Uint32** gPixels, Uint32*** gPixelArr2D, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture)
{
	//Free up allocated pixel array
	free(*gPixels);
	free(**gPixelArr2D);

	//Destroy texture
	SDL_DestroyTexture(*gTexture);
	*gTexture = NULL;

	//Destroy renderer
	SDL_DestroyRenderer(*gRenderer);
	*gRenderer = NULL;

	//Destroy window
	SDL_DestroyWindow(*gWindow);
	*gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void test(int argc, char argv[]) {

	//Screen dimension constants
	volatile int SCREEN_WIDTH = 640;
	volatile int SCREEN_HEIGHT = 480;

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Texture* gTexture = NULL;
	Uint32* gPixels = NULL;
	Uint32** gPixelArr2D = NULL;

	sdl_init(&gPixels, &gPixelArr2D, &gWindow, &gRenderer, &gTexture, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_RenderClear(gRenderer);

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			if ((sqrt(pow((320-x),2)+pow((240-y),2))) > (200-1) && (sqrt(pow((320 - x), 2) + pow((240 - y), 2))) < (200+1)) gPixels[y * SCREEN_WIDTH + x] = 0xFFFF0000;

		}
	}

	SDL_UpdateTexture(gTexture, NULL, gPixels, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	/*
	int x, y, r;
	r = 50;

	x = 100;
	y = 100;
	circleRGBA(gRenderer, x, y, r, 255, 0, 0, 255);
	circleRGBA(gRenderer, x + r, y, r, 0, 255, 0, 255);
	circleRGBA(gRenderer, x + r * cos(3.1415 / 3), y - r * sin(3.1415 / 3), r, 0, 0, 255, 255);

	x = 280;
	y = 100;
	aacircleRGBA(gRenderer, x, y, r, 255, 0, 0, 255);
	aacircleRGBA(gRenderer, x + r, y, r, 0, 255, 0, 255);
	aacircleRGBA(gRenderer, x + r * cos(3.1415 / 3), y - r * sin(3.1415 / 3), r, 0, 0, 255, 255);

	x = 100;
	y = 280;
	filledCircleRGBA(gRenderer, x, y, r, 255, 0, 0, 255);
	filledCircleRGBA(gRenderer, x + r, y, r, 0, 255, 0, 255);
	filledCircleRGBA(gRenderer, x + r * cos(3.1415 / 3), y - r * sin(3.1415 / 3), r, 0, 0, 255, 255);

	x = 280;
	y = 280;
	filledCircleRGBA(gRenderer, x, y, r, 255, 0, 0, 96);
	filledCircleRGBA(gRenderer, x + r, y, r, 0, 255, 0, 96);
	filledCircleRGBA(gRenderer, x + r * cos(3.1415 / 3), y - r * sin(3.1415 / 3), r, 0, 0, 255, 96);

	stringRGBA(gRenderer, 110, 350, "Kilepeshez: piros x az ablakon", 255, 255, 255, 255);

	SDL_RenderPresent(gRenderer);
	*/

	SDL_Event ev;
	while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
	}

	/* ablak bezarasa */
	sdl_close(&gPixels, &gPixelArr2D, &gWindow, &gRenderer, &gTexture);
}


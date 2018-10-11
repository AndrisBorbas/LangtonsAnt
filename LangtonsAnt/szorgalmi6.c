#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

//scan margin
const double eps = 1.5;

//colors in hex ARGB
/*enum colors {
	RED = 0xFFFF0000,
	GREEN = 0xFF00FF00, //rip iso c
	BLUE = 0xFF0000FF,
	WHITE = 0xFFFFFFFF
};*/

//Init SDL and variables
bool sdl_init6(Uint32**, SDL_Window**, SDL_Renderer**, SDL_Texture**, int, int);

//delete everything and close sdl
void sdl_close6(Uint32**, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//main
void pontok(int argc, char argv[]) {

	//Screen dimension constants
	volatile int SCREEN_WIDTH = 640;
	volatile int SCREEN_HEIGHT = 480;

	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Texture* gTexture = NULL;
	Uint32* gPixels = NULL;

	sdl_init6(&gPixels, &gWindow, &gRenderer, &gTexture, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_RenderClear(gRenderer);

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			int redD, greenD, blueD, whiteD1, whiteD2;
			//piros
			redD = (sqrt(pow((320 - x), 2) + pow((240 - y), 2)));
			if (redD > (200 - eps) && redD < (200 + eps)) gPixels[y * SCREEN_WIDTH + x] = 0xFFFF0000;
			//zöld
			greenD = (sqrt(pow((240 - x), 2) + pow((200 - y), 2)) + (sqrt(pow((400 - x), 2) + pow((280 - y), 2))));
			if (greenD > (250 - eps) && greenD < (250 + eps)) gPixels[y * SCREEN_WIDTH + x] = 0xFF00FF00;
			//kék
			blueD = (sqrt(pow((240 - x), 2) + pow((240 - y), 2)) - (sqrt(pow((400 - x), 2) + pow((240 - y), 2))));
			if (blueD < 0)blueD = -blueD;
			if (blueD > (100 - eps) && blueD < (100 + eps)) gPixels[y * SCREEN_WIDTH + x] = 0xFF0000FF;
			//fehér
			whiteD1 = (sqrt(pow((320 - x), 2) + pow((240 - y), 2)));
			whiteD2 = (sqrt(pow((400 - x), 2)));
			if (whiteD1 > (whiteD2 - eps) && whiteD1 < (whiteD2 + eps)) gPixels[y * SCREEN_WIDTH + x] = 0xFFFFFFFF;
		}
	}
	SDL_UpdateTexture(gTexture, NULL, gPixels, SCREEN_WIDTH * sizeof(Uint32));
	SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
	SDL_RenderPresent(gRenderer);


	SDL_Event ev;
	while (SDL_WaitEvent(&ev) && ev.type != SDL_QUIT) {
	}

	sdl_close6(&gPixels, &gWindow, &gRenderer, &gTexture);
}

bool sdl_init6(Uint32** gPixels, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture, int SCREEN_WIDTH, int SCREEN_HEIGHT)
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
		*gWindow = SDL_CreateWindow(".ok", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

			//Override all pixels to white
			memset(*gPixels, 255255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		}
	}

	return success;
}

void sdl_close6(Uint32** gPixels, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture)
{
	//Free up allocated pixel array
	free(*gPixels);

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
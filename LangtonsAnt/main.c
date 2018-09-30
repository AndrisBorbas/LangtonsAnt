#include <SDL.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Starts up SDL and creates window
//bool init(Uint32* pixels, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* gTexture);

//Frees media and shuts down SDL
//void close(Uint32* pixels, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* gTexture);

bool init(Uint32 *pixels, SDL_Window *gWindow, SDL_Renderer *gRenderer, SDL_Texture *gTexture)
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
		gWindow = SDL_CreateWindow("Langton's Ant", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Get window surface
			//gScreenSurface = SDL_GetWindowSurface(gWindow);

			//Initialize the renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, 0);

			//Initialize screen texture
			gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

			//Initialize pixel array
			pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(*pixels));

			//Override all pixels to white
			memset(pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
		}
	}

	return success;
}

void close(Uint32* pixels, SDL_Window* gWindow, SDL_Renderer* gRenderer, SDL_Texture* gTexture)
{
	free(pixels);

	//Destroy window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main(int argc, char  argv)
{
	bool quit = false;

	bool leftMouseButtonDown = false;

	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The pixel renderer
	SDL_Renderer* gRenderer = NULL;

	SDL_Texture* gTexture = NULL;
	Uint32* pixels = NULL;

	//Start up SDL and create window
	if (!init(&pixels, &gWindow, &gRenderer, &gTexture))
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		SDL_Event event;

		while (!quit)
		{
			SDL_UpdateTexture(gTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

			SDL_WaitEvent(&event);

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftMouseButtonDown = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
					leftMouseButtonDown = true;
			case SDL_MOUSEMOTION:
				if (leftMouseButtonDown)
				{
					int mouseX = event.motion.x;
					int mouseY = event.motion.y;
					printf("%d %d", mouseX, mouseY);
					pixels[mouseY * SCREEN_WIDTH + mouseX] = 0;
				}
				break;
			}

			SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}

	//Free resources and close SDL

	close(pixels, gWindow, gRenderer, gTexture);

	return 0;
}
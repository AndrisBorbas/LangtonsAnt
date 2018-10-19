#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

//memset with 32 bit unsigned integer
void memset32(void * dest, Uint32 value, uintptr_t size)
{
	uintptr_t i;
	for (i = 0; i < (size & (~3)); i += 4)
	{
		memcpy(((char*)dest) + i, &value, 4);
	}
	for (; i < size; i++)
	{
		((char*)dest)[i] = ((char*)&value)[i & 3];
	}
}

Uint32 ftick(Uint32 ms, void *param)
{
	SDL_Event ev;
	ev.type = SDL_USEREVENT;
	SDL_PushEvent(&ev);
	return ms;
}

//Width of the window
volatile int SCREEN_WIDTH = 960;
//Height of the window
volatile int SCREEN_HEIGHT = 960;
//Number of pixels per grid square
volatile int SCALE = 4;
//Number of pixels between grid squares
volatile int SPACING = 1;
//Number of pixels the ant is smaller than the grid (has to be at least 1)
volatile int ANTMARGIN = 1;

int lepes = 0;

enum AntHeading
{
	UP = 0,
	RIGHT = 90,
	DOWN = 180,
	LEFT = 270
};

enum HEXARGB
{
	WHITE = 0xFFFFFFFF,
	DARKWHITE = 0xFFDDDDDD,
	GRAY = 0xFF333333,
	BLACK = 0xFF000000
};

typedef struct Button {
	int x, y, width, height;
}Button;

//Stores the properties of the ant
typedef struct Ant
{
	int x, y, heading, lasttile;
}Ant;

//Frees media and shuts down SDL
void close(Uint32**, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Starts up SDL and creates window
bool initSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Initializes the pixel arrays
bool initPixels(Uint32**, Uint32***);

//Converts pixel texture to sdl pixel format
bool convertPixels(Uint32**, Uint32***);

//Check if mouse is inside a button
bool isMouseinButton(int, int, Button);

//The alogorithm for the ants movement
bool antgorithm(Uint32***, Ant*);

//move ant and invert past location
bool moveAnt(Uint32***, Ant*);

void ANTMAiN(int argc, char ** argv)
{
	Ant ant;
	ant.x = SCREEN_WIDTH / 2;
	ant.y = SCREEN_HEIGHT / 2;
	ant.heading = LEFT;
	ant.lasttile = DARKWHITE;

	Button startbutton;
	startbutton.width = 150;
	startbutton.height = 40;
	startbutton.x = SCREEN_WIDTH / 2 - startbutton.width / 2;
	startbutton.y = SCREEN_HEIGHT / 2 - startbutton.height / 2;

	//Handle normal quit
	bool quit = false;
	//Handle quit on error
	bool equit = false;
	//Start the algorithm
	bool start = false;
	//Let the algorithm run
	bool Running = false;
	//Is there an error
	bool ERROR = false;

	bool leftMouseButtonDown = false;

	//The window we'll be rendering to
	SDL_Window *gWindow = NULL;

	//The pixel renderer
	SDL_Renderer *gRenderer = NULL;

	SDL_Texture *tPixelTexture = NULL;
	SDL_Texture *tMainMenu = NULL;
	Uint32 *pixels = NULL;
	Uint32 **pixelTex = NULL;

	//Start up SDL and create window
	if (!initSDL(&gWindow, &gRenderer, &tPixelTexture))
	{
		printf("Failed to initialize: %s", SDL_GetError());
		exit(1);
	}

	//Load main menu background
	tMainMenu = IMG_LoadTexture(gRenderer, "MainMenu.png");
	if (tMainMenu == NULL)
	{
		SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
		exit(1);
	}
	//Render main menu
	SDL_RenderClear(gRenderer);
	SDL_RenderCopy(gRenderer, tMainMenu, NULL, NULL);
	SDL_RenderPresent(gRenderer);
	rectangleRGBA();

	SDL_Event event;

	while (!quit || !start)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				//Get mouse position
				int mouseX;
				int mouseY;
				SDL_GetMouseState(&mouseX, &mouseY);
				if (isMouseinButton(mouseX, mouseY, startbutton))start = true;
			}
			break;
		case SDL_MOUSEMOTION:
			if (leftMouseButtonDown)
			{
				//Get mouse position
				int mouseX = event.motion.x;
				int mouseY = event.motion.y;
				printf("X:%d Y:%d    ", mouseX, mouseY);

				//Set 2D texture
				pixelTex[mouseX][mouseY] = 0xFFFF0000;
				pixelTex[mouseX][mouseY + 1] = 0xFFFF0000;
				pixelTex[mouseX + 1][mouseY] = 0xFFFF0000;
				pixelTex[mouseX + 1][mouseY + 1] = 0xFFFF0000;

				//printf("%d  ", pixelTex[mouseX][mouseY]);

				//Convert 2D texture to 1D pixel array
				for (int x = 0; x < SCREEN_WIDTH; x++)
				{
					for (int y = 0; y < SCREEN_HEIGHT; y++)
					{
						pixels[x + y * SCREEN_WIDTH] = pixelTex[x][y];
					}
				}
				//pixels[mouseY * SCREEN_WIDTH + mouseX] = 0xFF00FF00;
				//printf("%d |", pixels[mouseY * SCREEN_WIDTH + mouseX]);
			}
			break;
		}
	}

	//Initialize pixel textures
	initPixels(&pixels, &pixelTex);

	//Create base tickrate
	SDL_TimerID tick = SDL_AddTimer(16, ftick, NULL);



	while (!quit)
	{
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_USEREVENT:
			while (Running)
			{
				if (!moveAnt(&pixelTex, &ant))
				{
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				if (!ERROR) {
					SDL_RenderClear(gRenderer);
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
						//if (Running)Running = false;
						/*else */Running = false;
						break;
					}
				}
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_g:
				if (!moveAnt(&pixelTex, &ant))
				{
					ERROR = true;
					quit = true;
				}
				convertPixels(&pixels, &pixelTex);
				SDL_UpdateTexture(tPixelTexture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
				//if (Running)Running = false;
				//else Running = true;
				break;
			case SDLK_SPACE:
				Running = true;
				break;
			}
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
			SDL_RenderClear(gRenderer);
			SDL_RenderCopy(gRenderer, tPixelTexture, NULL, NULL);
			SDL_RenderPresent(gRenderer);
		}
	}
	//Free resources and close SDL
	close(&pixels, &gWindow, &gRenderer, &tPixelTexture, &tMainMenu);
}

bool moveAnt(Uint32*** pixelTex, Ant* ant)
{
	if (!antgorithm(pixelTex, ant))return false;
	lepes++;
	printf("%d. lepes: ", lepes);

	//printf("%d  %d\n", ant->y, ant->y - SCALE / 2 + SPACING);
	//(*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + (ant->y - SCALE - SPACING + ANTMARGIN) * SCREEN_WIDTH] = 0xFF00FF00;

	//invert last location
	for (int i = (ant->x - SCALE); i < (ant->x + SCALE - SPACING); i++)
	{
		for (int j = (ant->y - SCALE); j < (ant->y + SCALE - SPACING); j++)
		{
			if (i<0 || i>SCREEN_WIDTH || j<0 || j>SCREEN_HEIGHT)
			{
				printf("ERROR: ant out of bounds at x=%d, y=%d.", i, j);
				return false;
			}
			switch (ant->lasttile)
			{
			case GRAY:
				(*pixelTex)[i][j] = DARKWHITE;
				break;
			case DARKWHITE:
				(*pixelTex)[i][j] = GRAY;
				break;
			default:
				break;
			}
		}
	}



	//printf("%d ", ant->heading);
	switch (ant->heading)
	{
	case UP:
		ant->y = ant->y - SCALE * 2;
		break;
	case DOWN:
		ant->y = ant->y + SCALE * 2;
		break;
	case RIGHT:
		ant->x = ant->x + SCALE * 2;
		break;
	case LEFT:
		ant->x = ant->x - SCALE * 2;
		break;
	default:
		break;
	}
	//printf("%d\n", ant->heading);
	for (int i = (ant->x - SCALE + ANTMARGIN); i < (ant->x + SCALE - SPACING - ANTMARGIN); i++)
	{
		for (int j = (ant->y - SCALE + ANTMARGIN); j < (ant->y + SCALE - SPACING - ANTMARGIN); j++)
		{
			if (i<0 || i>SCREEN_WIDTH || j<0 || j>SCREEN_HEIGHT)
			{
				printf("ERROR: ant out of bounds at x=%d, y=%d.", i, j);
				return false;
			}
			(*pixelTex)[i][j] = 0xFFFF0000;
		}
	}
	return true;
}

bool antgorithm(Uint32*** pixelTex, Ant* ant) {
	//printf("%u\n%u\n%u\n\n", ((*pixels)[(ant->x - SCALE - SPACING + ANTMARGIN) + ((ant->y - SCALE - SPACING + ANTMARGIN)*SCREEN_WIDTH)]), BLACK, 0xFF000000);
	int xpos = (ant->x - SCALE - SPACING + ANTMARGIN);
	int ypos = (ant->y - SCALE - SPACING + ANTMARGIN);

	if (xpos < 0 || xpos > SCREEN_WIDTH || ypos < 0 || ypos > SCREEN_HEIGHT)return false;

	//int position = xpos + (ypos * SCREEN_WIDTH);
	if (((*pixelTex)[xpos][ypos]) == BLACK || ((*pixelTex)[xpos][ypos]) == GRAY)
	{
		ant->heading = ant->heading + 90;
		if (ant->heading > 270) ant->heading = ant->heading - 360;
		ant->lasttile = GRAY;
		printf("jobb\n");
		return true;
	}
	if (((*pixelTex)[xpos][ypos]) == DARKWHITE)
	{
		ant->heading = ant->heading - 90;
		if (ant->heading < 0) ant->heading = ant->heading + 360;
		ant->lasttile = DARKWHITE;
		printf("bal\n");
		return true;
	}
	return false;
}

bool initSDL(SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** gTexture)
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
		}
	}
#ifdef __Win32__
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
#endif
	return success;
}

bool initPixels(Uint32** pixels, Uint32*** pixelarr) {
	//Initialize pixel array
	*pixels = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(**pixels));

	//Initialize 2D pixel texture array
	*pixelarr = (Uint32 ***)malloc(sizeof(Uint32 *) * SCREEN_WIDTH);
	*pixelarr[0] = (Uint32 **)malloc(sizeof(Uint32) * SCREEN_HEIGHT * SCREEN_WIDTH);
	for (int i = 0; i < SCREEN_WIDTH; i++)
		(*pixelarr)[i] = (**pixelarr + SCREEN_HEIGHT * i);

	//Override all pixels
	memset32(*pixels, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	memset32(**pixelarr, BLACK, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
	return true;
}

bool convertPixels(Uint32** pixels, Uint32*** pixelarr) {
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		for (int j = 0; j < SCREEN_HEIGHT; j++)
		{
			(*pixels)[i + j * SCREEN_WIDTH] = (*pixelarr)[i][j];
		}
	}
}

bool isMouseinButton(int mouseX, int mouseY, Button button)
{
	if (button.x > mouseX)return false;
	if (button.x + button.width < mouseX)return false;
	if (button.y > mouseY)return false;
	if (button.y + button.height < mouseY)return false;
	return true;
}

void close(Uint32** pixels, SDL_Window** gWindow, SDL_Renderer** gRenderer, SDL_Texture** tPixelTexture, SDL_Texture** tMainMenu)
{
	//Free up allocated pixel array
	free(*pixels);

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
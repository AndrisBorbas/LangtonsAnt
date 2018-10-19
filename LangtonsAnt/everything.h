#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxprimitives.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>


//Stores the properties of the ant
typedef struct Ant {
	int x, y, heading, lasttile;
}Ant;

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

enum HEXRGBA
{
	altWHITE = 0xFFFFFFFF,
	altDARKWHITE = 0xDDDDDDFF,
	altGRAY = 0x333333FF,
	altBLACK = 0x000000FF
};

//memset with 32 bit unsigned integer
void memset32(void*, Uint32, uintptr_t);

//Frees media and shuts down SDL
void close(Uint32**, Uint32***, SDL_Window**, SDL_Renderer**, SDL_Texture**);

//Starts up SDL and creates window
bool initSDL(SDL_Window**, SDL_Renderer**, SDL_Texture**, SDL_Texture**, int const, int const);

//Initializes the pixel arrays
bool initPixels(Uint32**, Uint32***, int const, int const);

//Converts pixel texture to sdl pixel format
bool convertPixels(Uint32**, Uint32***, int const, int const);

//Base tickrate of the simulation
Uint32 ftick(Uint32, void*);

//The alogorithm for the ants movement
bool antgorithm(Uint32***, Ant*, int const, int const, int const, int const, int);

//move ant and invert past location
bool moveAnt(Uint32***, Ant*, int const, int const, int const, int const);
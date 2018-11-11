#pragma once

#include "includes.h"

//memset with 32 bit unsigned integer
void memset32(void*, Uint32, uintptr_t);

//Converts pixel texture to sdl pixel format
bool convertPixels(Uint32**, Uint32***, int const, int const);

//Loads the configuration into int variables
void loadintFromConfig(FILE*, char*, int*, char*);

//Loads the configuration into char variables
void loadcharFromConfig(FILE*, char*, char*, char*);

//Base tickrate of the simulation
Uint32 ftick(Uint32, void*);
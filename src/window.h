#ifndef WINDOW_H
#define WINDOW_H

#include <SDL.h>
#include <SDL_syswm.h>

#include <iostream>

// Create new SDL window
SDL_Window* createWindow(int width, int height, const char * title);

#endif
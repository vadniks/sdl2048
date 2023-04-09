
#pragma once

#include <sdl/SDL.h>

extern const unsigned ROWS;
extern const unsigned COLUMNS;

void initRenderer(SDL_Renderer* renderer);
void doRender(SDL_Renderer* renderer);
void cleanRenderer();

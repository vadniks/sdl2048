
#pragma once

#include <sdl/SDL.h>

extern const unsigned ROWS;
extern const unsigned COLUMNS;

void rendererInit(SDL_Renderer* renderer);
unsigned* rendererFieldItems();
unsigned* rendererScore();
void rendererDraw();
void rendererClean();

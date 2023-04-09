
#pragma once

#include <sdl/SDL.h>
#include <stdbool.h>

void logicInit(bool* isGameRunning, unsigned* rendererFieldItems, unsigned* rendererScore);
void logicHandleEvent(SDL_Event* event);
void logicClean();

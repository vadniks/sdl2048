
#pragma once

#include <sdl/SDL.h>
#include <stdbool.h>

void logicInit(bool* gameRunningFlag, unsigned* rendererFieldItems);
void logicHandleEvent(SDL_Event* event);
void logicClean();

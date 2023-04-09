
#pragma once

#include <sdl/SDL.h>
#include <stdbool.h>

void logicInit(bool* gameRunningFlag);
void logicHandleEvent(SDL_Event* event);
void logicClean();

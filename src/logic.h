
#pragma once

#include <sdl/SDL.h>
#include "render.h"

void logicInit(
    bool* isGameRunning,
    unsigned* nums,
    unsigned* score,
    RenderResetButtonState* resetButtonState
);
void logicHandleEvent(SDL_Event* event);
void logicClean();

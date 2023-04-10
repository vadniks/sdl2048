
#pragma once

#include <sdl/SDL.h>
#include "render.h"

void logicInit(
    bool* isGameRunning,
    unsigned* rendererFieldItems,
    unsigned* rendererScore,
    RendererResetButtonState* rendererResetButtonState
);
void logicHandleEvent(SDL_Event* event);
void logicClean();

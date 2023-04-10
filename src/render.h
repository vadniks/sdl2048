
#pragma once

#include <sdl/SDL.h>
#include <stdbool.h>

extern const unsigned ROWS, COLUMNS, IGNORED_NUM;

typedef struct {
    SDL_Rect* geometry;
    int coordsMultiplier;
    bool* isPressed;
} RendererResetButtonState;

void rendererInit(SDL_Renderer* renderer);
unsigned* rendererFieldItems();
unsigned* rendererScore();
RendererResetButtonState* rendererResetButtonState();
void rendererDraw();
void rendererClean();

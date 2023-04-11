
#pragma once

#include <sdl/SDL.h>
#include <stdbool.h>

extern const unsigned ROWS, COLUMNS, IGNORED_NUM, END_NUM;

typedef struct {
    SDL_Rect* geometry;
    int scaleMultiplier;
    bool* isPressed;
} RenderResetButtonState;

void renderInit(SDL_Renderer* renderer);
unsigned* renderFieldItems();
unsigned* renderScore();
RenderResetButtonState* renderResetButtonState();
void renderMarkItemSpecial(unsigned index);
void renderClearSpecialItemMarks();
void renderOnUpdate();
void renderDraw();
void renderClean();

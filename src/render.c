
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 5;

unsigned* gField = NULL;
unsigned gWidth = 0, gHeight = 0, gTileSize = 0;

void setDrawColorToDefault(SDL_Renderer* renderer)
{ SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255); }

void initRenderer(SDL_Renderer* renderer) {
    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    SDL_GetRendererOutputSize(renderer, (signed*) &gWidth, (signed*) &gHeight);
    assert(gWidth == gHeight && ROWS == COLUMNS);
    gTileSize = gWidth / ROWS / THICKNESS;
}

void drawWindowFrame(SDL_Renderer* renderer) {
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = (signed) (gWidth / THICKNESS);
    rect.h = (signed) (gHeight / THICKNESS);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float) THICKNESS, (float) THICKNESS);
    SDL_RenderDrawRect(renderer, &rect);
}

void drawField(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float) THICKNESS, (float) THICKNESS);

    const int end = (signed) (gHeight / THICKNESS);
    for (unsigned i = 0; i < gWidth / THICKNESS - THICKNESS; i += gTileSize)
        SDL_RenderDrawLine(renderer, (signed) i, 0, (signed) i, end),
        SDL_RenderDrawLine(renderer, 0, (signed) i, end, (signed) i); //TODO
}

void doRender(SDL_Renderer* renderer) {
    setDrawColorToDefault(renderer);
    SDL_RenderClear(renderer);

//    drawWindowFrame(renderer);
    drawField(renderer);

    SDL_RenderPresent(renderer);
}

void cleanRenderer() {
    SDL_free(gField);
}

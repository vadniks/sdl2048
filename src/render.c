
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4;

unsigned* gField = NULL;
unsigned gWidth = 0, gHeight = 0, gTileSize = 0;

void setDrawColorToDefault(SDL_Renderer* renderer)
{ SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255); }

void initRenderer(SDL_Renderer* renderer) {
    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    SDL_GetRendererOutputSize(renderer, (signed*) &gWidth, (signed*) &gHeight);
    assert(gWidth == gHeight && ROWS == COLUMNS);
    gTileSize = (gWidth - (ROWS + 1) * THICKNESS) / ROWS;
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

    const int end = (signed) (ROWS * gTileSize / THICKNESS + THICKNESS / 2);
    for (unsigned i = 0; i < ROWS + 1; i++) {
        int each = (signed) (i * gTileSize / THICKNESS + THICKNESS / 2);
        SDL_RenderDrawLine(renderer, each, THICKNESS / 2, each, end);
        SDL_RenderDrawLine(renderer, THICKNESS / 2, each, end, each);
    }

//    const int end = (signed) (gHeight / THICKNESS), start = 0/*(signed) THICKNESS / 2*/;
//    for (unsigned i = start; i < gWidth / THICKNESS; i += gTileSize + THICKNESS)
//        SDL_RenderDrawLine(renderer, (signed) i, start, (signed) i, end - start),
//        SDL_RenderDrawLine(renderer, start, (signed) i, end - start, (signed) i);
}

void doRender(SDL_Renderer* renderer) {
    setDrawColorToDefault(renderer);
    SDL_RenderClear(renderer);

    drawWindowFrame(renderer);
    drawField(renderer);

    SDL_RenderPresent(renderer);
}

void cleanRenderer() {
    SDL_free(gField);
}

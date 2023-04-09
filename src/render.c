
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4;
const unsigned COLUMNS = ROWS;
unsigned* gField = NULL;
unsigned width = 0, height = 0, tileSize = 0;

void initRenderer(SDL_Renderer* renderer) {
    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    SDL_GetRendererOutputSize(renderer, (signed*) &width, (signed*) &height);
    assert(width == height && ROWS == COLUMNS);
    tileSize = width / ROWS;

    SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255);
}

void drawWindowFrame(SDL_Renderer* renderer) {
    const unsigned thickness = 5;
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = (signed) (width / thickness);
    rect.h = (signed) (height / thickness);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float) thickness, (float) thickness);
    SDL_RenderDrawRect(renderer, &rect);
}

void doRender(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255);
    SDL_RenderClear(renderer);

    drawWindowFrame(renderer);

    SDL_RenderPresent(renderer);
}

void cleanRenderer() {
    SDL_free(gField);
}


#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4;

unsigned* gField = NULL;
unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0, gFieldEnd = 0;

void setDrawColorToDefault(SDL_Renderer* renderer)
{ SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255); }

void initRenderer(SDL_Renderer* renderer) {
    assert(ROWS == COLUMNS);
    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    SDL_GetRendererOutputSize(renderer, (signed*) &gWidth, (signed*) &gHeight);
    gFieldSize = gWidth / 2;
    gTileSize = (gFieldSize - (ROWS + 1) * THICKNESS) / ROWS;

    gFieldStart = (signed) THICKNESS / 2;
    gFieldEnd = (signed) (ROWS * gTileSize / THICKNESS + gFieldStart);
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

    for (unsigned i = 0; i < ROWS + 1; i++) {
        const int each = (signed) (i * gTileSize / THICKNESS + gFieldStart);
        SDL_RenderDrawLine(renderer, each, (signed) gFieldStart, each, (signed) gFieldEnd);
        SDL_RenderDrawLine(renderer, (signed) gFieldStart, each, (signed) gFieldEnd, each);
    }
}

void drawInfo(SDL_Renderer* renderer) {
    // TODO
}

void doRender(SDL_Renderer* renderer) {
    setDrawColorToDefault(renderer);
    SDL_RenderClear(renderer);

    drawWindowFrame(renderer);
    drawField(renderer);
    drawInfo(renderer);

    SDL_RenderPresent(renderer);
}

void cleanRenderer() {
    SDL_free(gField);
}

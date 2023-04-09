
#include "render.h"

const unsigned ROWS = 4;
const unsigned COLUMNS = ROWS;
unsigned* gField = NULL;
unsigned width = 0, height = 0;

void initRenderer(SDL_Renderer* renderer) {
    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);
    SDL_GetRendererOutputSize(renderer, (signed*) &width, (signed*) &height);

    SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255);
}

void doRender(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void cleanRenderer() {
    SDL_free(gField);
}

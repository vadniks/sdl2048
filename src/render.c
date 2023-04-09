
#include "render.h"

void initRenderer(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void doRender(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

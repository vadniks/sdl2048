
#include <stdlib.h>
#include <sdl/SDL.h>

int main(int argc, char** argv) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO))
        return EXIT_FAILURE;

    window = SDL_CreateWindow("Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) return EXIT_FAILURE;

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) return EXIT_FAILURE;

    SDL_RendererInfo rendererInfo;
    SDL_GetRendererInfo(renderer, &rendererInfo);
    SDL_Log("Renderer: %s", rendererInfo.name);

    int a = SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    if (a != 0) return EXIT_FAILURE;
    int b = SDL_RenderClear(renderer);
    if (b != 0) return EXIT_FAILURE;
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}

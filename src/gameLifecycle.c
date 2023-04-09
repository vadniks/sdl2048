
#include <sdl/SDL.h>
#include "gameLifecycle.h"

SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
bool gRunning = false;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO))
        return false;

    gWindow = SDL_CreateWindow(
        "Title",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        SDL_WINDOW_SHOWN
    );
    if (!gWindow) return false;

    gRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (!gRenderer) return false;

    gRunning = true;
    return true;
}

void handleEvents() {
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return;

    switch (event.type) {
        case SDL_QUIT:
            gRunning = false;
            break;
        default: break;
    }
}

void update() {

}

void render() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
    SDL_RenderPresent(gRenderer);
}

void clean() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool loop() {
    if (!init()) return false;

    while (gRunning) {
        handleEvents();
        update();
        render();
    }

    clean();
    return true;
}

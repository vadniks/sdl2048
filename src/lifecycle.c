
#include <sdl/SDL.h>
#include "lifecycle.h"
#include "render.h"

const unsigned WIDTH = 640;
const unsigned HEIGHT = WIDTH / 2;

SDL_Window* gWindow = NULL;
SDL_Renderer* gSdlRenderer = NULL;
bool gRunning = false;

bool gameInit() {
    if (SDL_Init(SDL_INIT_VIDEO))
        return false;

    gWindow = SDL_CreateWindow(
        "Title",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (signed) WIDTH,
        (signed) HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!gWindow) return false;

    gSdlRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    if (!gSdlRenderer) return false;

    rendererInit(gSdlRenderer);

    gRunning = true;
    return true;
}

void gameHandleEvents() {
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return;

    switch (event.type) {
        case SDL_QUIT:
            gRunning = false;
            break;
        default: break;
    }
}

void gameUpdate() {

}

void gameRender() { rendererDraw(); }

void gameClean() {
    rendererClean();
    SDL_DestroyRenderer(gSdlRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool gameLoop() {
    if (!gameInit()) {
        gameClean();
        return false;
    }

    while (gRunning) {
        gameHandleEvents();
        gameUpdate();
        gameRender();
    }

    gameClean();
    return true;
}

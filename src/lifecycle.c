
#include <sdl/SDL.h>
#include "lifecycle.h"
#include "render.h"
#include "logic.h"

const unsigned WIDTH = 800;
const unsigned HEIGHT = WIDTH / 2;

SDL_Window* gWindow = NULL;
SDL_Renderer* gSdlRenderer = NULL;
bool gRunning = false;

bool gameInit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        return false;

    gWindow = SDL_CreateWindow(
        "2048 clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (signed) WIDTH,
        (signed) HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!gWindow) return false;

    gSdlRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gSdlRenderer) return false;

    rendererInit(gSdlRenderer);
    logicInit(&gRunning, rendererFieldItems(), rendererScore(), rendererResetButtonState());

    gRunning = true;
    return true;
}

void gameHandleEvents() {
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return;
    logicHandleEvent(&event);
}

void gameUpdate() {
    // TODO: make fixed update (continuous timer task with fixed delay) or remove this at all
}

void gameRender() { rendererDraw(); }

void gameClean() {
    rendererClean();
    logicClean();
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

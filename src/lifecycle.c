
#include <sdl/SDL.h>
#include "lifecycle.h"
#include "render.h"
#include "logic.h"

const unsigned WIDTH = 800;
const unsigned HEIGHT = WIDTH / 2;

SDL_Window* gLifecycleWindow = NULL;
SDL_Renderer* gLifecycleRenderer = NULL;
bool gLifecycleRunning = false;

bool gameInit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
        return false;

    gLifecycleWindow = SDL_CreateWindow(
        "2048 clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (signed) WIDTH,
        (signed) HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!gLifecycleWindow) return false;

    gLifecycleRenderer = SDL_CreateRenderer(gLifecycleWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gLifecycleRenderer) return false;

    renderInit(gLifecycleRenderer);
    logicInit(&gLifecycleRunning, renderFieldItems(), renderScore(), renderResetButtonState());

    gLifecycleRunning = true;
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

void gameRender() { renderDraw(); }

void gameClean() {
    renderClean();
    logicClean();
    SDL_DestroyRenderer(gLifecycleRenderer);
    SDL_DestroyWindow(gLifecycleWindow);
    SDL_Quit();
}

bool gameLoop() {
    if (!gameInit()) {
        gameClean();
        return false;
    }

    while (gLifecycleRunning) {
        gameHandleEvents();
        gameUpdate();
        gameRender();
        SDL_Delay(1000 / 60);
    }

    gameClean();
    return true;
}

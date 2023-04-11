
#include <sdl/SDL.h>
#include "lifecycle.h"
#include "render.h"
#include "logic.h"

const unsigned WIDTH = 800, HEIGHT = WIDTH / 2, UPDATE_DELAY = 1000 / 20;

SDL_Window* gLifecycleWindow = NULL;
SDL_Renderer* gLifecycleRenderer = NULL;
bool gLifecycleRunning = false;
SDL_TimerID gLifecycleUpdateTimerId = 0;

unsigned lifecycleUpdate(unsigned, void*);
void lifecycleShowWinDialog();

bool lifecycleInit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER))
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
    logicInit(&gLifecycleRunning, renderFieldItems(), renderScore(), renderResetButtonState(), &lifecycleShowWinDialog);

    gLifecycleUpdateTimerId = SDL_AddTimer(UPDATE_DELAY, &lifecycleUpdate, NULL);

    gLifecycleRunning = true;
    return true;
}

void lifecycleShowWinDialog() { SDL_ShowSimpleMessageBox(
    SDL_MESSAGEBOX_INFORMATION,
    "Info",
    "You have won!",
    gLifecycleWindow
); }

unsigned lifecycleUpdate(__attribute__((unused)) unsigned _, __attribute__((unused)) void* __) { // NOLINT(bugprone-reserved-identifier)
    renderOnUpdate();
    return gLifecycleRunning ? UPDATE_DELAY : 0;
}

void lifecycleHandleEvents() {
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return;
    logicHandleEvent(&event);
}

void lifecycleRender() { renderDraw(); }

void lifecycleClean() {
    SDL_RemoveTimer(gLifecycleUpdateTimerId);
    renderClean();
    logicClean();
    SDL_DestroyRenderer(gLifecycleRenderer);
    SDL_DestroyWindow(gLifecycleWindow);
    SDL_Quit();
}

bool lifecycleLoop() {
    if (!lifecycleInit()) {
        lifecycleClean();
        return false;
    }

    while (gLifecycleRunning) {
        lifecycleHandleEvents();
        lifecycleRender();
        SDL_Delay(1000 / 60);
    }

    lifecycleClean();
    return true;
}

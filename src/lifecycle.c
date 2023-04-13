
#include <sdl/SDL.h>
#include "lifecycle.h"
#include "render.h"
#include "logic.h"

const unsigned WIDTH = 800, HEIGHT = WIDTH / 2, UPDATE_DELAY = 1000 / 20;

static SDL_Window* gWindow = NULL;
static SDL_Renderer* gRenderer = NULL;
static bool gRunning = false;
static SDL_TimerID gUpdateTimerId = 0;

unsigned lifecycleUpdate(unsigned, void*);
void lifecycleShowWinDialog();

bool lifecycleInit() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_TIMER))
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

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (!gRenderer) return false;

    renderInit(gRenderer);
    logicInit(&gRunning, renderFieldItems(), renderScore(), renderResetButtonState(), &lifecycleShowWinDialog);

    gUpdateTimerId = SDL_AddTimer(UPDATE_DELAY, &lifecycleUpdate, NULL);

    gRunning = true;
    return true;
}

void lifecycleShowWinDialog() { SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION,
        "Info",
        "You have won!",
        gWindow
); }

unsigned lifecycleUpdate(__attribute__((unused)) unsigned _, __attribute__((unused)) void* __) { // NOLINT(bugprone-reserved-identifier)
    renderOnUpdate();
    return gRunning ? UPDATE_DELAY : 0;
}

void lifecycleHandleEvents() {
    SDL_Event event;
    if (!SDL_PollEvent(&event)) return;
    logicHandleEvent(&event);
}

void lifecycleRender() { renderDraw(); }

void lifecycleClean() {
    SDL_RemoveTimer(gUpdateTimerId);
    renderClean();
    logicClean();
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

bool lifecycleLoop() {
    if (!lifecycleInit()) {
        lifecycleClean();
        return false;
    }

    while (gRunning) {
        lifecycleHandleEvents();
        lifecycleRender();
        SDL_Delay(1000 / 60);
    }

    lifecycleClean();
    return true;
}


#include "logic.h"
#include "render.h"

bool* gIsRunning = NULL;
unsigned* gRendererFieldItems = NULL;
unsigned* gRendererScore = NULL;

void logicInit(bool* isGameRunning, unsigned* rendererFieldItems, unsigned* rendererScore) {
    gIsRunning = isGameRunning;
    gRendererFieldItems = rendererFieldItems;
    gRendererScore = rendererScore;
}

void processKeyboardButtonPress(SDL_Keycode keycode) {
    switch (keycode) {
        case SDLK_w:
            SDL_Log("w\n"); // TODO: test only
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 1);
            *gRendererScore = 1;
            break;
        case SDLK_a:
            SDL_Log("a\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 2);
            *gRendererScore = 2;
            break;
        case SDLK_s:
            SDL_Log("s\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 3);
            *gRendererScore = 3;
            break;
        case SDLK_d:
            SDL_Log("d\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 4);
            *gRendererScore = 4;
            break;
        default: break;
    }
}

void logicHandleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            *gIsRunning = false;
            break;
        case SDL_KEYDOWN:
            processKeyboardButtonPress(event->key.keysym.sym);
            break;
        default: break;
    }
}

void logicClean() {

}

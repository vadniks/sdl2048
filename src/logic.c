
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
        case SDLK_w: // TODO: test only
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 1);
            *gRendererScore = 1;
            break;
        case SDLK_a:
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 16);
            *gRendererScore = 16;
            break;
        case SDLK_s:
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 32);
            *gRendererScore = 32;
            break;
        case SDLK_d:
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 2048);
            *gRendererScore = 2048;
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

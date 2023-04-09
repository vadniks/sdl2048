
#include "logic.h"
#include "render.h"

bool* gIsRunning = NULL;
unsigned* gRendererFieldItems = NULL;

void logicInit(bool* gameRunningFlag, unsigned* rendererFieldItems) {
    gIsRunning = gameRunningFlag;
    gRendererFieldItems = rendererFieldItems;
}

void processKeyboardButtonPress(SDL_Keycode keycode) {
//    SDL_Log("%s\n", SDL_GetKeyName(keycode));
    switch (keycode) {
        case SDLK_w:
            SDL_Log("w\n"); // TODO: test only
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 1);
            break;
        case SDLK_a:
            SDL_Log("a\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 2);
            break;
        case SDLK_s:
            SDL_Log("s\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 3);
            break;
        case SDLK_d:
            SDL_Log("d\n");
            for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 4);
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

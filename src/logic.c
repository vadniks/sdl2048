
#include "logic.h"

bool* gIsRunning = NULL;
unsigned* gRendererFieldItems = NULL;

void logicInit(bool* gameRunningFlag, unsigned* rendererFieldItems) {
    gIsRunning = gameRunningFlag;
    gRendererFieldItems = rendererFieldItems;
}

void logicHandleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            *gIsRunning = false;
            break;
        case SDL_KEYDOWN:
            SDL_Log("%s\n", SDL_GetKeyName(event->key.keysym.sym));
            break;
        default: break;
    }
}

void logicClean() {

}

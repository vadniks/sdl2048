
#include "logic.h"

bool* gIsRunning = NULL;

void logicInit(bool* gameRunningFlag) {
    gIsRunning = gameRunningFlag;
}

void logicHandleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            *gIsRunning = false;
            break;
        default:
            break;
    }
}

void logicClean() {

}

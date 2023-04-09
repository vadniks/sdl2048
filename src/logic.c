
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

#define KEY_EVENT_TEST(x) \
    for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = (x)); \
        *gRendererScore = x;

void processKeyboardButtonPress(SDL_Keycode keycode) {
    switch (keycode) {
        case SDLK_w: // TODO: test only
            KEY_EVENT_TEST(2)
            break;
        case SDLK_a:
            KEY_EVENT_TEST(16)
            break;
        case SDLK_s:
            KEY_EVENT_TEST(128)
            break;
        case SDLK_d:
            KEY_EVENT_TEST(2048)
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

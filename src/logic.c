
#include "logic.h"

bool* gIsRunning = NULL;
unsigned* gRendererFieldItems = NULL;
unsigned* gRendererScore = NULL;
RendererResetButtonState* gRendererResetButtonState = NULL;

void logicInit(
    bool* isGameRunning,
    unsigned* rendererFieldItems,
    unsigned* rendererScore,
    RendererResetButtonState* rendererResetButtonState
) {
    gIsRunning = isGameRunning;
    gRendererFieldItems = rendererFieldItems;
    gRendererScore = rendererScore;
    gRendererResetButtonState = rendererResetButtonState;
}

#define KEY_EVENT_TEST(x) \
    for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = (x)); \
        *gRendererScore = x;

void processKeyboardButtonPress(SDL_Keycode keycode) {
    switch (keycode) {
        case SDLK_w: // TODO: test only
            KEY_EVENT_TEST(2)
            *gRendererResetButtonState->isPressed = true; // TODO: test only
            break;
        case SDLK_a:
            KEY_EVENT_TEST(16)
            *gRendererResetButtonState->isPressed = false;
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
    SDL_free(gRendererResetButtonState);
}

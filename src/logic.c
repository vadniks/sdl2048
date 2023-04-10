
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

bool isMouseWithinResetButtonArea() {
    int mouseX = -1, mouseY = -1;
    SDL_GetMouseState(&mouseX, &mouseY);

    int margin = gRendererResetButtonState->margin;
    mouseX /= margin;
    mouseY /= margin;

    SDL_Rect* buttonRect = gRendererResetButtonState->geometry;
    SDL_Log("%d %d | %d %d %d %d\n", mouseX, mouseY, buttonRect->x, buttonRect->y, buttonRect->w, buttonRect->h); // TODO
    return mouseX >= buttonRect->x
        && mouseX <= buttonRect->x + buttonRect->w
        && mouseY >= buttonRect->y
        && mouseY <= buttonRect->y + buttonRect->h;
}

void logicHandleEvent(SDL_Event* event) {
#   define SET_RESET_BUTTON_PRESSED(x) *(gRendererResetButtonState->isPressed) = x;
    switch (event->type) {
        case SDL_QUIT:
            *gIsRunning = false;
            break;
        case SDL_KEYDOWN:
            processKeyboardButtonPress(event->key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            SDL_Log("a %c\n", isMouseWithinResetButtonArea() ? 't' : 'f');
            if (isMouseWithinResetButtonArea())
                SET_RESET_BUTTON_PRESSED(true)
            break;
        case SDL_MOUSEBUTTONUP:
            SET_RESET_BUTTON_PRESSED(false)
        default: break;
    }
#   undef SET_RESET_BUTTON_PRESSED
}

void logicClean() {
    SDL_free(gRendererResetButtonState);
}

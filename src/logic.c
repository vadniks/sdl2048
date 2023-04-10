
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "logic.h"

const unsigned NEW_NUMS_COUNT = 2, NEW_NUM_VALUE = 2;

bool* gIsRunning = NULL;
unsigned* gRendererFieldItems = NULL;
unsigned* gRendererScore = NULL;
RendererResetButtonState* gRendererResetButtonState = NULL;
unsigned gMaxSpawnIterations = 0;

void initGame();

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

    initGame();
}

void spawnNew(unsigned iteration);

void initGame() {
    srand(time(NULL)); // NOLINT(cert-msc51-cpp)
    gMaxSpawnIterations = ROWS * COLUMNS - 1;
    spawnNew(0);
}

unsigned coordsToIndex(unsigned row, unsigned column) {
    assert(row < ROWS && column < COLUMNS);
    return row * COLUMNS + column;
}

#define NUM_AT(r, c) gRendererFieldItems[coordsToIndex(r, c)]

void spawnNew(unsigned iteration) {
    if (iteration >= NEW_NUMS_COUNT || iteration > gMaxSpawnIterations) return;

#   define RAND rand() % ROWS
    unsigned row = RAND, column = RAND; // NOLINT(cert-msc50-cpp)
#   undef RAND

    bool successful = false; // TODO: create array of allowed cells
    if (NUM_AT(row, column) == IGNORED_NUM) {
        successful = true;
        NUM_AT(row, column) = NEW_NUM_VALUE;
    }

    spawnNew(iteration + (successful ? 1 : 0));
}

void processKeyboardButtonPress(SDL_Keycode keycode) {
    switch (keycode) {
        case SDLK_w:

            break;
        case SDLK_a:

            break;
        case SDLK_s:

            break;
        case SDLK_d:

            break;
        default: break;
    }
}

bool isMouseWithinResetButtonArea() {
    int mouseX = -1, mouseY = -1;
    SDL_GetMouseState(&mouseX, &mouseY);

    int coordsMultiplier = gRendererResetButtonState->coordsMultiplier;
    mouseX /= coordsMultiplier;
    mouseY /= coordsMultiplier;

    SDL_Rect* buttonRect = gRendererResetButtonState->geometry;
    return mouseX >= buttonRect->x
        && mouseX <= buttonRect->x + buttonRect->w
        && mouseY >= buttonRect->y
        && mouseY <= buttonRect->y + buttonRect->h;
}

void onResetButtonEventReceived(bool down) {
    *(gRendererResetButtonState->isPressed) = down;
    if (!down) return;

    for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = 0);
    *gRendererScore = 0;
}

void logicHandleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            *gIsRunning = false;
            break;
        case SDL_KEYDOWN:
            processKeyboardButtonPress(event->key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (isMouseWithinResetButtonArea())
                onResetButtonEventReceived(true);
            break;
        case SDL_MOUSEBUTTONUP:
            onResetButtonEventReceived(false);
        default: break;
    }
}

void logicClean() {
    SDL_free(gRendererResetButtonState);
}

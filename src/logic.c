
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
unsigned gNumsCount = 0;

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
    gNumsCount = ROWS * COLUMNS;
    spawnNew(0);
}

unsigned coordsToIndex(unsigned row, unsigned column) {
    assert(row < ROWS && column < COLUMNS);
    return row * COLUMNS + column;
}

// TODO: display newly spawned nums with different color

#define NUM_AT(x, y) gRendererFieldItems[coordsToIndex(x, y)]

typedef struct {
    unsigned x;
    unsigned y;
} Coords;

void spawnNew(unsigned iteration) {
    if (iteration >= NEW_NUMS_COUNT || iteration > gMaxSpawnIterations) return;

    NUM_AT(1, 3) = 5;

    for (unsigned i = 0; i < gNumsCount; i++)
        gRendererFieldItems[i] = i;

    Coords* emptyCoords = NULL;
    unsigned emptyCoordsSize = 0, x = 0, y = 0;
    for (unsigned i = 0; i < gNumsCount; i++) {
        if (gRendererFieldItems[i] != IGNORED_NUM) {
            SDL_Log("%u %u %u\n", i, x, y);
//        emptyCoords = SDL_realloc(emptyCoords, ++emptyCoordsSize);
//        emptyCoords[emptyCoordsSize - 1] = (Coords) {  };
        }

        if (i > 0 && i % COLUMNS == 0) {
            y = 0;
            x++;
        }
        y++;
    }

#   define RAND rand() % ROWS
    unsigned row = RAND, column = RAND; // NOLINT(cert-msc50-cpp)
#   undef RAND

    bool successful = false;
    if (NUM_AT(row, column) == IGNORED_NUM) {
        successful = true;
//        NUM_AT(row, column) = NEW_NUM_VALUE;
    }

    if (emptyCoordsSize > 0)
        SDL_free(emptyCoords);

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

    int coordsMultiplier = gRendererResetButtonState->scaleMultiplier;
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

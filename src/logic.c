
#include <stdlib.h>
#include <time.h>
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
    srand(time(NULL));
    gMaxSpawnIterations = ROWS * COLUMNS - 1;
    gNumsCount = ROWS * COLUMNS;
    spawnNew(0);
}

void spawnNew(unsigned iteration) {
    if (iteration >= NEW_NUMS_COUNT || iteration > gMaxSpawnIterations) return;
    if (iteration == 0) rendererClearSpecialFieldItemMarks();

    unsigned* emptyIndexes = NULL;
    unsigned emptyIndexesSize = 0;

    for (unsigned i = 0, x = 0, y = 0; i < gNumsCount; i++, y++) {
        if (gRendererFieldItems[i] == IGNORED_NUM) {
            emptyIndexes = SDL_realloc(emptyIndexes, sizeof(unsigned) * ++emptyIndexesSize);
            emptyIndexes[emptyIndexesSize - 1] = i;
        }
        if (i > 0 && i % COLUMNS == 0) {
            y = 0;
            x++;
        }
    }

    if (!emptyIndexesSize) return;
    const unsigned emptyIndex = emptyIndexes[rand() % emptyIndexesSize];
    bool successful = false;

    if (gRendererFieldItems[emptyIndex] == IGNORED_NUM) {
        successful = true;
        gRendererFieldItems[emptyIndex] = NEW_NUM_VALUE;
        rendererMarkFieldItemSpecial(emptyIndex);
    }

    SDL_free(emptyIndexes);
    spawnNew(iteration + (successful ? 1 : 0));
}

void shiftNumsUp() {
    unsigned before[COLUMNS];
    for (unsigned i = 0; i < COLUMNS; before[i] = gRendererFieldItems[i * COLUMNS], i++);

    for (int y = (signed) ROWS - 1, x, index = 0; y >= 0; y--) {
        for (x = 0; x < COLUMNS; x++, index++) {
            if (y - 1 >= 0) gRendererFieldItems[x * COLUMNS + y - 1] *= gRendererFieldItems[x * COLUMNS + y];
            if (y > 0) gRendererFieldItems[x * COLUMNS + y] = IGNORED_NUM;
        }
    }

    for (unsigned i = 0, index; i < COLUMNS; i++) {
        index = i * COLUMNS;
        if ((before[i] + IGNORED_NUM) < gRendererFieldItems[index]) (*gRendererScore)++;
    }
}

void processKeyboardButtonPress(SDL_Keycode keycode) {
    bool needToSpawnNew = true;
    switch (keycode) {
        case SDLK_w:
            shiftNumsUp();
            break;
        case SDLK_a:
            // TODO
            break;
        case SDLK_s:

            break;
        case SDLK_d:

            break;
        default:
            needToSpawnNew = false;
            break;
    }
    if (needToSpawnNew) spawnNew(0);
}

bool isMouseWithinResetButtonArea() {
    int mouseX = -1, mouseY = -1;
    SDL_GetMouseState(&mouseX, &mouseY);

    int scaleMultiplier = gRendererResetButtonState->scaleMultiplier;
    mouseX /= scaleMultiplier;
    mouseY /= scaleMultiplier;

    SDL_Rect* buttonRect = gRendererResetButtonState->geometry;
    return mouseX >= buttonRect->x
        && mouseX <= buttonRect->x + buttonRect->w
        && mouseY >= buttonRect->y
        && mouseY <= buttonRect->y + buttonRect->h;
}

void onResetButtonEventReceived(bool down) {
    *(gRendererResetButtonState->isPressed) = down;
    if (!down) return;

    for (unsigned i = 0; i < ROWS * COLUMNS; gRendererFieldItems[i++] = IGNORED_NUM);
    *gRendererScore = 0;
    spawnNew(0);
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

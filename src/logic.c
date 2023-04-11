
#include <stdlib.h>
#include <time.h>
#include "logic.h"

const unsigned NEW_NUMS_COUNT = 2, NEW_NUM_VALUE = 2;

bool* gLogicIsRunning = NULL;
unsigned* gLogicNums = NULL;
unsigned* gLogicScore = NULL;
RenderResetButtonState* gLogicResetButtonState = NULL;
unsigned gLogicMaxSpawnIterations = 0;
unsigned gLogicNumsCount = 0;
unsigned* gLogicNumsShifted = NULL;

void logicInitGame();

void logicInit(
    bool* isGameRunning,
    unsigned* nums,
    unsigned* score,
    RenderResetButtonState* resetButtonState
) {
    gLogicIsRunning = isGameRunning;
    gLogicNums = nums;
    gLogicScore = score;
    gLogicResetButtonState = resetButtonState;

    logicInitGame();
}

void logicSpawnNew(unsigned iteration);

void logicInitGame() {
    srand(time(NULL));

    gLogicMaxSpawnIterations = ROWS * COLUMNS - 1;
    gLogicNumsCount = ROWS * COLUMNS;
    gLogicNumsShifted = SDL_calloc(ROWS * COLUMNS, sizeof *gLogicNumsShifted);

    logicSpawnNew(0);
}

void logicSpawnNew(unsigned iteration) {
    if (iteration >= NEW_NUMS_COUNT || iteration > gLogicMaxSpawnIterations) return;
    if (iteration == 0) renderClearSpecialItemMarks();

    unsigned* emptyIndexes = NULL;
    unsigned emptyIndexesSize = 0;

    for (unsigned i = 0, x = 0, y = 0; i < gLogicNumsCount; i++, y++) {
        if (gLogicNums[i] == IGNORED_NUM) {
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

    if (gLogicNums[emptyIndex] == IGNORED_NUM) {
        successful = true;
        gLogicNums[emptyIndex] = NEW_NUM_VALUE;
        renderMarkItemSpecial(emptyIndex);
    }

    SDL_free(emptyIndexes);
    logicSpawnNew(iteration + (successful ? 1 : 0));
}

void logicShiftProxy(void (*shift)(void)) {
    SDL_memcpy(gLogicNumsShifted, gLogicNums, gLogicNumsCount);
    shift();
}

#define NUM_AT(y, x) gLogicNums[(x) * (signed) COLUMNS + (y)]
#define SHIFTED_AT(y, x) gLogicNumsShifted[(x) * (signed) COLUMNS + (y)]

void logicShiftNumsUp() {
    bool summed = false;
    unsigned sum, current, shifted;

    for (int y = 0, x, nextY; y < ROWS; y++) {
        for (x = 0; x < COLUMNS; x++) {

            current = NUM_AT(y, x);
            if (current == IGNORED_NUM) continue;

            sum = current;
            nextY = y - 1;
            summed = false;

            while (nextY > -1) {
                shifted = SHIFTED_AT(nextY, x);

                if (NUM_AT(nextY, x) == current && (shifted == current || shifted == IGNORED_NUM)) {
                    sum += current;
                    NUM_AT(nextY, x) = IGNORED_NUM;
                    summed = true;
                } else if (NUM_AT(nextY, x) != IGNORED_NUM)
                    break;

                nextY--;
            }

            NUM_AT(y, x) = IGNORED_NUM;
            NUM_AT(nextY + 1, x) = sum;

            if (summed) *gLogicScore += sum;
            SHIFTED_AT(nextY + 1, x) = current;
        }
    }
}

void logicShiftNumsLeft() {
//    for (int x = (signed) COLUMNS - 1, y; x >= 0 ; x--) {
//        for (y = 0; y < ROWS; y++) {
//            if (x - 1 >= 0) gLogicNums[(x - 1) * COLUMNS + y] *= gLogicNums[x * COLUMNS + y];
//            if (x > 0) gLogicNums[x * COLUMNS + y] = IGNORED_NUM;
//        }
//    }
}

void logicProcessKeyboardButtonPress(SDL_Keycode keycode) {
    bool needToSpawnNew = true;
    switch (keycode) {
        case SDLK_w:
            logicShiftProxy(&logicShiftNumsUp);
            break;
        case SDLK_a:
            logicShiftProxy(&logicShiftNumsLeft);
            break;
        case SDLK_s:

            break;
        case SDLK_d:

            break;
        default:
            needToSpawnNew = false;
            break;
    }
    if (needToSpawnNew) logicSpawnNew(0);
}

bool logicIsMouseWithinResetButtonArea() {
    int mouseX = -1, mouseY = -1;
    SDL_GetMouseState(&mouseX, &mouseY);

    int scaleMultiplier = gLogicResetButtonState->scaleMultiplier;
    mouseX /= scaleMultiplier;
    mouseY /= scaleMultiplier;

    SDL_Rect* buttonRect = gLogicResetButtonState->geometry;
    return mouseX >= buttonRect->x
        && mouseX <= buttonRect->x + buttonRect->w
        && mouseY >= buttonRect->y
        && mouseY <= buttonRect->y + buttonRect->h;
}

void logicOnResetButtonEventReceived(bool down) {
    *(gLogicResetButtonState->isPressed) = down;
    if (!down) return;

    for (unsigned i = 0; i < ROWS * COLUMNS; gLogicNums[i++] = IGNORED_NUM);
    *gLogicScore = 0;
    logicSpawnNew(0);
}

void logicHandleEvent(SDL_Event* event) {
    switch (event->type) {
        case SDL_QUIT:
            *gLogicIsRunning = false;
            break;
        case SDL_KEYDOWN:
            logicProcessKeyboardButtonPress(event->key.keysym.sym);
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (logicIsMouseWithinResetButtonArea())
                logicOnResetButtonEventReceived(true);
            break;
        case SDL_MOUSEBUTTONUP:
            logicOnResetButtonEventReceived(false);
        default: break;
    }
}

void logicClean() {
    SDL_free(gLogicResetButtonState);
    SDL_free(gLogicNumsShifted);
}

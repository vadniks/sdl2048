
#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4, MAX_NUM_LENGTH = 4, MAX_NUM_VALUE = 9999,
    RESET_BUTTON_WIDTH = 80, RESET_BUTTON_HEIGHT = 30, RESET_BUTTON_BORDER_THICKNESS = 2, IGNORED_NUM = 1,
    CURRENT_SCORE_TEXT_WIDTH = 175, CURRENT_SCORE_TEXT_HEIGHT = 30, END_NUM = 2048;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

static unsigned* gItems = NULL;

static unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0,
    gFieldEnd = 0;

static TTF_Font* gFont = NULL;
static SDL_Color* gTextColor = NULL;
static SDL_Renderer* gRenderer = NULL;
static unsigned gScore = 0;
static bool gIsResetButtonPressed = false;
static SDL_Rect* gResetButtonGeometry = NULL;
static SDL_Color* gSpecialItemColor = NULL;
static unsigned* gSpecialItems = NULL;
static unsigned gSpecialItemsSize = 0;
static unsigned char gFrameColorComponent = 0;
static bool gFrameColorChangeDirection = false;
static unsigned gTitleXPos = 0;
static bool gTitleMovementDirection = false;
static unsigned gTitleOriginalXPos = 0;
static unsigned gTileOffset = 0;

void renderSetDrawColorToDefault()
{ SDL_SetRenderDrawColor(gRenderer, 49, 54, 59, 255); }

void renderInit(SDL_Renderer* renderer) {
    assert(ROWS == COLUMNS);
    gRenderer = renderer;
    SDL_GetRendererOutputSize(gRenderer, (signed*) &gWidth, (signed*) &gHeight);

    gFieldSize = gWidth / 2;
    gTileSize = (gFieldSize - (ROWS + 1) * THICKNESS) / ROWS;
    gFieldStart = (signed) THICKNESS / 2;
    gFieldEnd = (signed) (ROWS * gTileSize / THICKNESS + gFieldStart);

    gItems = SDL_calloc(ROWS * COLUMNS, sizeof(unsigned));
    for (unsigned i = 0; i < ROWS * COLUMNS; gItems[i++] = IGNORED_NUM);

    TTF_Init();
    gFont = TTF_OpenFont(FONT_PATH, 100);

    gTextColor = SDL_calloc(1, sizeof *gTextColor);
    gTextColor->a = 255;

    gResetButtonGeometry = SDL_malloc(sizeof *gResetButtonGeometry);
    gResetButtonGeometry->x = (signed) ((gFieldSize * 3 / 2 - RESET_BUTTON_WIDTH / 2) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->y = (signed) (((THICKNESS * 2) + 30) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->w = (signed) (RESET_BUTTON_WIDTH / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->h = (signed) (RESET_BUTTON_HEIGHT / RESET_BUTTON_BORDER_THICKNESS);

    gSpecialItemColor = SDL_malloc(sizeof *gSpecialItemColor);
    gSpecialItemColor->r = 30;
    gSpecialItemColor->g = 30;
    gSpecialItemColor->b = 30;
    gSpecialItemColor->a = 255;

    gTitleOriginalXPos = gFieldSize * 3 / 2 + THICKNESS - (CURRENT_SCORE_TEXT_WIDTH / 2);
    gTitleXPos = gTitleOriginalXPos;
    gTileOffset = CURRENT_SCORE_TEXT_WIDTH / 2;
}

unsigned* renderFieldItems() { return gItems; }

unsigned* renderScore() { return &gScore; }

RenderResetButtonState* renderResetButtonState() {
    RenderResetButtonState* state = SDL_malloc(sizeof *state);
    state->geometry = gResetButtonGeometry;
    state->scaleMultiplier = (signed) RESET_BUTTON_BORDER_THICKNESS;
    state->isPressed = &gIsResetButtonPressed;
    return state;
}

void renderMarkItemSpecial(unsigned index) {
    assert(index <= ROWS * COLUMNS);
    gSpecialItems = SDL_realloc(gSpecialItems, sizeof(unsigned) * ++gSpecialItemsSize);
    gSpecialItems[gSpecialItemsSize - 1] = index;
}

void renderClearSpecialItemMarks() {
    SDL_free(gSpecialItems);
    gSpecialItems = NULL;
    gSpecialItemsSize = 0;
}

void renderNextFrameColor() {
    if (!gFrameColorChangeDirection && gFrameColorComponent == 255
        || gFrameColorChangeDirection && gFrameColorComponent == 0)
        gFrameColorChangeDirection = !gFrameColorChangeDirection;

    if (!gFrameColorChangeDirection)
        gFrameColorComponent += 5;
    else
        gFrameColorComponent -= 5;
}

void renderNextTitleXPos() {
    if (!gTitleMovementDirection && gTitleXPos == gTitleOriginalXPos - gTileOffset
        || gTitleMovementDirection && gTitleXPos == gTitleOriginalXPos + gTileOffset)
        gTitleMovementDirection = !gTitleMovementDirection;

    if (!gTitleMovementDirection)
        gTitleXPos--;
    else
        gTitleXPos++;
}

void renderOnUpdate() {
    renderNextFrameColor();
    renderNextTitleXPos();
}

void renderDrawWindowFrame() {
    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gWidth / THICKNESS),
        (signed) (gHeight / THICKNESS)
    };

    SDL_SetRenderDrawColor(
        gRenderer,
        gFrameColorComponent,
        gFrameColorComponent,
        gFrameColorComponent,
        255
    );

    SDL_RenderSetScale(gRenderer, (float) THICKNESS, (float) THICKNESS);
    SDL_RenderDrawRect(gRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
}

SDL_Texture* renderMakeTextTexture(char* text, SDL_Color* color) {
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text, *color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

char* renderNumToText(int num) {
    char* text = SDL_calloc(MAX_NUM_LENGTH, sizeof(char));
    SDL_itoa(num, text, 10);
    return text;
}

void renderDrawNum(SDL_Rect* rect, int num, SDL_Color* color) {
    assert(num >= 0 && num <= MAX_NUM_VALUE);

    char* text = renderNumToText(num);
    SDL_Texture* texture = renderMakeTextTexture(text, color);
    SDL_RenderCopy(gRenderer, texture, NULL, rect);

    SDL_DestroyTexture(texture);
    SDL_free(text);
}

int renderCalcFieldNumCoord(unsigned logicalCoord)
{ return (signed) ((logicalCoord * gTileSize / THICKNESS + gFieldStart) * THICKNESS); }

bool renderIsSpecialFieldItem(unsigned index) {
    for (unsigned i = 0; i < gSpecialItemsSize; i++)
        if (gSpecialItems[i] == index)
            return true;
    return false;
}

void renderDrawField() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderer, (float) THICKNESS, (float) THICKNESS);

    for (unsigned i = 0; i < ROWS + 1; i++) {
        const int each = (signed) (i * gTileSize / THICKNESS + gFieldStart);
        SDL_RenderDrawLine(gRenderer, each, (signed) gFieldStart, each, (signed) gFieldEnd);
        SDL_RenderDrawLine(gRenderer, (signed) gFieldStart, each, (signed) gFieldEnd, each);
    }

    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gTileSize - THICKNESS * 2),
        (signed) (gTileSize - THICKNESS * 2)
    };
    SDL_RenderSetScale(gRenderer, 1, 1);

    for (unsigned row = 0, column, index, item; row < ROWS; row++) {
        for (column = 0; column < COLUMNS; column++) {
            index = row * COLUMNS + column;
            item = gItems[index];
            if (item == IGNORED_NUM) continue;

            rect.x = renderCalcFieldNumCoord(row) + (signed) THICKNESS;
            rect.y = renderCalcFieldNumCoord(column);
            renderDrawNum(&rect, (signed) item, renderIsSpecialFieldItem(index) ? gSpecialItemColor : gTextColor);
        }
    }
}

void renderDrawCurrentScore() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize * 3 / 2 + THICKNESS - (CURRENT_SCORE_TEXT_WIDTH / 2)),
        (signed) THICKNESS,
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    const unsigned scoreMsgLength = 15, maxLength = scoreMsgLength + MAX_NUM_LENGTH + 1;
    char* scoreText = renderNumToText((signed) gScore);
    char* buffer = SDL_calloc(sizeof(char), maxLength);

    SDL_strlcat(buffer, "Current score: ", maxLength);
    SDL_strlcat(buffer, scoreText, maxLength);

    SDL_Texture* texture = renderMakeTextTexture(buffer, gTextColor);
    SDL_RenderSetScale(gRenderer, 1, 1);
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);

    SDL_free(scoreText);
    SDL_free(buffer);
    SDL_DestroyTexture(texture);
}

void renderDrawResetButton() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderer, (float) RESET_BUTTON_BORDER_THICKNESS, (float) RESET_BUTTON_BORDER_THICKNESS);
    SDL_RenderDrawRect(gRenderer, gResetButtonGeometry);

    SDL_Rect* rect = SDL_malloc(sizeof *rect);
    SDL_memcpy(rect, gResetButtonGeometry, sizeof *rect);

#   define CHANGE_VALUE(x, y) rect->x = rect->x * (signed) RESET_BUTTON_BORDER_THICKNESS + (y);
    CHANGE_VALUE(x, 2)
    CHANGE_VALUE(y, 2)
    CHANGE_VALUE(w, -4)
    CHANGE_VALUE(h, -4)
#   undef CHANGE_VALUE

#   define SET_COLOR(r, g, b) SDL_SetRenderDrawColor(gRenderer, r, g, b, 255)
    gIsResetButtonPressed
        ? SET_COLOR(49, 54, 59)
        : SET_COLOR(56, 62, 73);
#   undef SET_COLOR

    SDL_RenderSetScale(gRenderer, 1, 1);
    SDL_RenderFillRect(gRenderer, rect);

    SDL_Texture* texture = renderMakeTextTexture("Reset", gTextColor);
    SDL_RenderCopy(gRenderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_free(rect);
}

void renderDrawTitle() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) gTitleXPos,
        (signed) (gHeight - THICKNESS - 30),
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    SDL_Texture* texture = renderMakeTextTexture("2048 clone", gTextColor);
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void renderDraw() {
    renderSetDrawColorToDefault();
    SDL_RenderClear(gRenderer);

    renderDrawWindowFrame();
    renderDrawField();
    renderDrawCurrentScore();
    renderDrawResetButton();
    renderDrawTitle();

    SDL_RenderPresent(gRenderer);
}

void renderClean() {
    SDL_free(gItems);
    TTF_CloseFont(gFont);
    SDL_free(gTextColor);
    SDL_free(gResetButtonGeometry);
    SDL_free(gSpecialItemColor);
    SDL_free(gSpecialItems);
    TTF_Quit();
}

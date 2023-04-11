
#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4, MAX_NUM_LENGTH = 4, MAX_NUM_VALUE = 2048,
    RESET_BUTTON_WIDTH = 80, RESET_BUTTON_HEIGHT = 30, RESET_BUTTON_BORDER_THICKNESS = 2, IGNORED_NUM = 1,
    CURRENT_SCORE_TEXT_WIDTH = 175, CURRENT_SCORE_TEXT_HEIGHT = 30, RED_MASK = 0xff000000, GREEN_MASK = 0x00ff0000,
    BLUE_MASK = 0x0000ff00, RED_SHIFT = 0x18, GREEN_SHIFT = 0x10, BLUE_SHIFT = 0x08;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

unsigned* gRenderItems = NULL;

unsigned gRenderWidth = 0, gRenderHeight = 0, gRenderFieldSize = 0, gRenderTileSize = 0, gRenderFieldStart = 0,
    gRenderFieldEnd = 0;

TTF_Font* gRenderFont = NULL;
SDL_Color* gRenderTextColor = NULL;
SDL_Renderer* gRenderRenderer = NULL;
unsigned gRenderScore = 0;
bool gRenderIsResetButtonPressed = false;
SDL_Rect* gRenderResetButtonGeometry = NULL;
SDL_Color* gRenderSpecialItemColor = NULL;
unsigned* gRenderSpecialItems = NULL;
unsigned gRenderSpecialItemsSize = 0;
unsigned gRenderFrameColor = 0x00000000;

void renderSetDrawColorToDefault()
{ SDL_SetRenderDrawColor(gRenderRenderer, 49, 54, 59, 255); }

void renderInit(SDL_Renderer* renderer) {
    assert(ROWS == COLUMNS);
    gRenderRenderer = renderer;
    SDL_GetRendererOutputSize(gRenderRenderer, (signed*) &gRenderWidth, (signed*) &gRenderHeight);

    gRenderFieldSize = gRenderWidth / 2;
    gRenderTileSize = (gRenderFieldSize - (ROWS + 1) * THICKNESS) / ROWS;
    gRenderFieldStart = (signed) THICKNESS / 2;
    gRenderFieldEnd = (signed) (ROWS * gRenderTileSize / THICKNESS + gRenderFieldStart);

    gRenderItems = SDL_calloc(ROWS * COLUMNS, sizeof(unsigned));
    for (unsigned i = 0; i < ROWS * COLUMNS; gRenderItems[i++] = IGNORED_NUM);

    TTF_Init();
    gRenderFont = TTF_OpenFont(FONT_PATH, 100);

    gRenderTextColor = SDL_calloc(1, sizeof *gRenderTextColor);
    gRenderTextColor->a = 255;

    gRenderResetButtonGeometry = SDL_malloc(sizeof *gRenderResetButtonGeometry);
    gRenderResetButtonGeometry->x = (signed) ((gRenderFieldSize * 3 / 2 - RESET_BUTTON_WIDTH / 2) / RESET_BUTTON_BORDER_THICKNESS);
    gRenderResetButtonGeometry->y = (signed) (((THICKNESS * 2) + 30) / RESET_BUTTON_BORDER_THICKNESS);
    gRenderResetButtonGeometry->w = (signed) (RESET_BUTTON_WIDTH / RESET_BUTTON_BORDER_THICKNESS);
    gRenderResetButtonGeometry->h = (signed) (RESET_BUTTON_HEIGHT / RESET_BUTTON_BORDER_THICKNESS);

    gRenderSpecialItemColor = SDL_malloc(sizeof *gRenderSpecialItemColor);
    gRenderSpecialItemColor->r = 30;
    gRenderSpecialItemColor->g = 30;
    gRenderSpecialItemColor->b = 30;
    gRenderSpecialItemColor->a = 255;
}

unsigned* renderFieldItems() { return gRenderItems; }

unsigned* renderScore() { return &gRenderScore; }

RenderResetButtonState* renderResetButtonState() {
    RenderResetButtonState* state = SDL_malloc(sizeof *state);
    state->geometry = gRenderResetButtonGeometry;
    state->scaleMultiplier = (signed) RESET_BUTTON_BORDER_THICKNESS;
    state->isPressed = &gRenderIsResetButtonPressed;
    return state;
}

void renderMarkItemSpecial(unsigned index) {
    assert(index <= ROWS * COLUMNS);
    gRenderSpecialItems = SDL_realloc(gRenderSpecialItems, sizeof(unsigned) * ++gRenderSpecialItemsSize);
    gRenderSpecialItems[gRenderSpecialItemsSize - 1] = index;
}

void renderClearSpecialItemMarks() {
    SDL_free(gRenderSpecialItems);
    gRenderSpecialItems = NULL;
    gRenderSpecialItemsSize = 0;
}

void renderNextFrameColor() {
    unsigned current = gRenderFrameColor & BLUE_MASK;

    SDL_Log("0x%08x\n", current);

    if (current >= 0x0000ff00) current = 0x00000000;
    else current += 0x00000100;

    gRenderFrameColor |= current << RED_SHIFT;
    gRenderFrameColor |= current << GREEN_SHIFT;
    gRenderFrameColor |= current << BLUE_SHIFT; // TODO: make it work

    SDL_Log("0x%08x 0x%08x 0x%08x 0x%08x\n", gRenderFrameColor, (gRenderFrameColor & RED_MASK) >> RED_SHIFT, (gRenderFrameColor & GREEN_MASK) >> GREEN_SHIFT, (gRenderFrameColor & BLUE_MASK) >> BLUE_SHIFT);
}

void renderDrawWindowFrame() {
    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gRenderWidth / THICKNESS),
        (signed) (gRenderHeight / THICKNESS)
    };

    SDL_SetRenderDrawColor(
        gRenderRenderer,
        (gRenderFrameColor & RED_MASK) >> RED_SHIFT,
        (gRenderFrameColor & GREEN_MASK) >> GREEN_SHIFT,
        (gRenderFrameColor & BLUE_MASK) >> BLUE_SHIFT,
        255
    );

    SDL_RenderSetScale(gRenderRenderer, (float) THICKNESS, (float) THICKNESS);
    SDL_RenderDrawRect(gRenderRenderer, &rect);
    SDL_SetRenderDrawColor(gRenderRenderer, 0, 0, 0, 255);
}

SDL_Texture* renderMakeTextTexture(char* text, SDL_Color* color) {
    SDL_Surface* surface = TTF_RenderText_Solid(gRenderFont, text, *color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderRenderer, surface);
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
    SDL_RenderCopy(gRenderRenderer, texture, NULL, rect);

    SDL_DestroyTexture(texture);
    SDL_free(text);
}

int renderCalcFieldNumCoord(unsigned logicalCoord)
{ return (signed) ((logicalCoord * gRenderTileSize / THICKNESS + gRenderFieldStart) * THICKNESS); }

bool renderIsSpecialFieldItem(unsigned index) {
    for (unsigned i = 0; i < gRenderSpecialItemsSize; i++)
        if (gRenderSpecialItems[i] == index)
            return true;
    return false;
}

void renderDrawField() {
    SDL_SetRenderDrawColor(gRenderRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderRenderer, (float) THICKNESS, (float) THICKNESS);

    for (unsigned i = 0; i < ROWS + 1; i++) {
        const int each = (signed) (i * gRenderTileSize / THICKNESS + gRenderFieldStart);
        SDL_RenderDrawLine(gRenderRenderer, each, (signed) gRenderFieldStart, each, (signed) gRenderFieldEnd);
        SDL_RenderDrawLine(gRenderRenderer, (signed) gRenderFieldStart, each, (signed) gRenderFieldEnd, each);
    }

    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gRenderTileSize - THICKNESS * 2),
        (signed) (gRenderTileSize - THICKNESS * 2)
    };
    SDL_RenderSetScale(gRenderRenderer, 1, 1);

    for (unsigned row = 0, column, index, item; row < ROWS; row++) {
        for (column = 0; column < COLUMNS; column++) {
            index = row * COLUMNS + column;
            item = gRenderItems[index];
            if (item == IGNORED_NUM) continue;

            rect.x = renderCalcFieldNumCoord(row) + (signed) THICKNESS;
            rect.y = renderCalcFieldNumCoord(column);
            renderDrawNum(&rect, (signed) item, renderIsSpecialFieldItem(index) ? gRenderSpecialItemColor : gRenderTextColor);
        }
    }
}

void renderDrawCurrentScore() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gRenderFieldSize * 3 / 2 + THICKNESS - (CURRENT_SCORE_TEXT_WIDTH / 2)),
        (signed) THICKNESS,
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    const unsigned scoreMsgLength = 15, maxLength = scoreMsgLength + MAX_NUM_LENGTH + 1;
    char* scoreText = renderNumToText((signed) gRenderScore);
    char* buffer = SDL_calloc(sizeof(char), maxLength);

    SDL_strlcat(buffer, "Current score: ", maxLength);
    SDL_strlcat(buffer, scoreText, maxLength);

    SDL_Texture* texture = renderMakeTextTexture(buffer, gRenderTextColor);
    SDL_RenderSetScale(gRenderRenderer, 1, 1);
    SDL_RenderCopy(gRenderRenderer, texture, NULL, &rect);

    SDL_free(scoreText);
    SDL_free(buffer);
    SDL_DestroyTexture(texture);
}

void renderDrawResetButton() {
    SDL_SetRenderDrawColor(gRenderRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderRenderer, (float) RESET_BUTTON_BORDER_THICKNESS, (float) RESET_BUTTON_BORDER_THICKNESS);
    SDL_RenderDrawRect(gRenderRenderer, gRenderResetButtonGeometry);

    SDL_Rect* rect = SDL_malloc(sizeof *rect);
    SDL_memcpy(rect, gRenderResetButtonGeometry, sizeof *rect);

#   define CHANGE_VALUE(x, y) rect->x = rect->x * (signed) RESET_BUTTON_BORDER_THICKNESS + (y);
    CHANGE_VALUE(x, 2)
    CHANGE_VALUE(y, 2)
    CHANGE_VALUE(w, -4)
    CHANGE_VALUE(h, -4)
#   undef CHANGE_VALUE

#   define SET_COLOR(r, g, b) SDL_SetRenderDrawColor(gRenderRenderer, r, g, b, 255)
    gRenderIsResetButtonPressed
        ? SET_COLOR(49, 54, 59)
        : SET_COLOR(56, 62, 73);
#   undef SET_COLOR

    SDL_RenderSetScale(gRenderRenderer, 1, 1);
    SDL_RenderFillRect(gRenderRenderer, rect);

    SDL_Texture* texture = renderMakeTextTexture("Reset", gRenderTextColor);
    SDL_RenderCopy(gRenderRenderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_free(rect);
}

void renderDrawTitle() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gRenderFieldSize * 3 / 2 + THICKNESS - (CURRENT_SCORE_TEXT_WIDTH / 2)),
        (signed) (gRenderHeight - THICKNESS - 30),
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    SDL_Texture* texture = renderMakeTextTexture("2048 clone", gRenderTextColor);
    SDL_RenderCopy(gRenderRenderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void renderDraw() {
    renderSetDrawColorToDefault();
    SDL_RenderClear(gRenderRenderer);

    renderDrawWindowFrame();
    renderDrawField();
    renderDrawCurrentScore();
    renderDrawResetButton();
    renderDrawTitle();

    SDL_RenderPresent(gRenderRenderer);
}

void renderClean() {
    SDL_free(gRenderItems);
    TTF_CloseFont(gRenderFont);
    SDL_free(gRenderTextColor);
    SDL_free(gRenderResetButtonGeometry);
    SDL_free(gRenderSpecialItemColor);
    SDL_free(gRenderSpecialItems);
    TTF_Quit();
}

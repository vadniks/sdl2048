
#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4, MAX_NUM_LENGTH = 5, MAX_NUM_VALUE = 16384,
    RESET_BUTTON_WIDTH = 80, RESET_BUTTON_HEIGHT = 30, RESET_BUTTON_BORDER_THICKNESS = 2, IGNORED_NUM = 1,
    CURRENT_SCORE_TEXT_WIDTH = 175, CURRENT_SCORE_TEXT_HEIGHT = 30;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

unsigned* gFieldItems = NULL;
unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0, gFieldEnd = 0;
TTF_Font* gFont = NULL;
SDL_Color* gTextColor = NULL;
SDL_Renderer* gRenderer = NULL;
unsigned gScore = 0;
bool gIsResetButtonPressed = false;
SDL_Rect* gResetButtonGeometry = NULL;
SDL_Color* gSpecialFieldItemColor = NULL;
unsigned* gSpecialFieldItems = NULL;
unsigned gSpecialFieldItemsSize = 0;

void setDrawColorToDefault()
{ SDL_SetRenderDrawColor(gRenderer, 49, 54, 59, 255); }

void rendererInit(SDL_Renderer* renderer) {
    assert(ROWS == COLUMNS);
    gRenderer = renderer;
    SDL_GetRendererOutputSize(gRenderer, (signed*) &gWidth, (signed*) &gHeight);

    gFieldSize = gWidth / 2;
    gTileSize = (gFieldSize - (ROWS + 1) * THICKNESS) / ROWS;
    gFieldStart = (signed) THICKNESS / 2;
    gFieldEnd = (signed) (ROWS * gTileSize / THICKNESS + gFieldStart);

    gFieldItems = SDL_calloc(ROWS * COLUMNS, sizeof(unsigned));
    for (unsigned i = 0; i < ROWS * COLUMNS; gFieldItems[i++] = IGNORED_NUM);

    TTF_Init();
    gFont = TTF_OpenFont(FONT_PATH, 100);

    gTextColor = SDL_calloc(1, sizeof *gTextColor);
    gTextColor->a = 255;

    gResetButtonGeometry = SDL_malloc(sizeof *gResetButtonGeometry);
    gResetButtonGeometry->x = (signed) ((gFieldSize + THICKNESS) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->y = (signed) (((THICKNESS * 2) + 30) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->w = (signed) (RESET_BUTTON_WIDTH / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->h = (signed) (RESET_BUTTON_HEIGHT / RESET_BUTTON_BORDER_THICKNESS);

    gSpecialFieldItemColor = SDL_malloc(sizeof *gSpecialFieldItemColor);
    gSpecialFieldItemColor->r = 30;
    gSpecialFieldItemColor->g = 30;
    gSpecialFieldItemColor->b = 30;
    gSpecialFieldItemColor->a = 255;
}

unsigned* rendererFieldItems() { return gFieldItems; }

unsigned* rendererScore() { return &gScore; }

RendererResetButtonState* rendererResetButtonState() {
    RendererResetButtonState* state = SDL_malloc(sizeof *state);
    state->geometry = gResetButtonGeometry;
    state->scaleMultiplier = (signed) RESET_BUTTON_BORDER_THICKNESS;
    state->isPressed = &gIsResetButtonPressed;
    return state;
}

void rendererMarkFieldItemSpecial(unsigned index) {
    gSpecialFieldItems = SDL_realloc(gSpecialFieldItems, sizeof(unsigned) * ++gSpecialFieldItemsSize);
    gSpecialFieldItems[gSpecialFieldItemsSize - 1] = index;
}

void rendererClearSpecialFieldItemMarks() {
    SDL_free(gSpecialFieldItems);
    gSpecialFieldItems = NULL;
    gSpecialFieldItemsSize = 0;
}

void drawWindowFrame() {
    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gWidth / THICKNESS),
        (signed) (gHeight / THICKNESS)
    };

    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderer, (float) THICKNESS, (float) THICKNESS);
    SDL_RenderDrawRect(gRenderer, &rect);
}

SDL_Texture* makeTextTexture(char* text, SDL_Color* color) {
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text, *color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

char* numToText(int num) {
    char* text = SDL_calloc(MAX_NUM_LENGTH, sizeof(char));
    SDL_itoa(num, text, 10);
    return text;
}

void drawNum(SDL_Rect* rect, int num, SDL_Color* color) {
    assert(num >= 0 && num <= MAX_NUM_VALUE);

    char* text = numToText(num);
    SDL_Texture* texture = makeTextTexture(text, color);
    SDL_RenderCopy(gRenderer, texture, NULL, rect);

    SDL_DestroyTexture(texture);
    SDL_free(text);
}

int calcFieldNumCoord(unsigned logicalCoord)
{ return (signed) ((logicalCoord * gTileSize / THICKNESS + gFieldStart) * THICKNESS); }

bool isSpecialFieldItem(unsigned index) {
    for (unsigned i = 0; i < gSpecialFieldItemsSize; i++)
        if (gSpecialFieldItems[i] == index)
            return true;
    return false;
}

void drawField() {
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
            item = gFieldItems[index];
            if (item == IGNORED_NUM) continue;

            rect.x = calcFieldNumCoord(row) + (signed) THICKNESS;
            rect.y = calcFieldNumCoord(column);
            drawNum(&rect, (signed) item, isSpecialFieldItem(index) ? gSpecialFieldItemColor : gTextColor);
        }
    }
}

void drawCurrentScore() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize + THICKNESS),
        (signed) THICKNESS,
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    const unsigned scoreMsgLength = 15, maxLength = scoreMsgLength + MAX_NUM_LENGTH + 1;
    char* scoreText = numToText((signed) gScore);
    char* buffer = SDL_calloc(sizeof(char), maxLength);

    SDL_strlcat(buffer, "Current score: ", maxLength);
    SDL_strlcat(buffer, scoreText, maxLength);

    SDL_Texture* texture = makeTextTexture(buffer, gTextColor);
    SDL_RenderSetScale(gRenderer, 1, 1);
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);

    SDL_free(scoreText);
    SDL_free(buffer);
    SDL_DestroyTexture(texture);
}

void drawResetButton() {
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

    SDL_Texture* texture = makeTextTexture("Reset", gTextColor);
    SDL_RenderCopy(gRenderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_free(rect);
}

void drawTitle() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize + THICKNESS + (CURRENT_SCORE_TEXT_WIDTH / 2)),
        (signed) (gHeight - THICKNESS - 30),
        (signed) CURRENT_SCORE_TEXT_WIDTH,
        (signed) CURRENT_SCORE_TEXT_HEIGHT
    };

    SDL_Texture* texture = makeTextTexture("2048 clone", gTextColor);
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}

void rendererDraw() {
    setDrawColorToDefault();
    SDL_RenderClear(gRenderer);

    drawWindowFrame();
    drawField();
    drawCurrentScore();
    drawResetButton();
    drawTitle();

    SDL_RenderPresent(gRenderer);
}

void rendererClean() {
    SDL_free(gFieldItems);
    TTF_CloseFont(gFont);
    SDL_free(gTextColor);
    SDL_free(gResetButtonGeometry);
    SDL_free(gSpecialFieldItemColor);
    SDL_free(gSpecialFieldItems);
    TTF_Quit();
}

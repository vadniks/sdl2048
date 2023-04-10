
#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4, MAX_NUM_LENGTH = 4, MAX_NUM_VALUE = 2048,
    RESET_BUTTON_WIDTH = 80, RESET_BUTTON_HEIGHT = 30, RESET_BUTTON_BORDER_THICKNESS = 2, IGNORED_NUM = 0;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

unsigned* gFieldItems = NULL;
unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0, gFieldEnd = 0;
TTF_Font* gFont = NULL;
SDL_Color* gTextColor = NULL;
SDL_Renderer* gRenderer = NULL;
unsigned gScore = 0;
bool gIsResetButtonPressed = false;
SDL_Rect* gResetButtonGeometry = NULL;

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

    TTF_Init();
    gFont = TTF_OpenFont(FONT_PATH, 100);

    gTextColor = SDL_calloc(1, sizeof *gTextColor);
    gTextColor->a = 255;

    gResetButtonGeometry = SDL_malloc(sizeof *gResetButtonGeometry);
    gResetButtonGeometry->x = (signed) ((gFieldSize + THICKNESS) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->y = (signed) (((THICKNESS * 2) + 30) / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->w = (signed) (RESET_BUTTON_WIDTH / RESET_BUTTON_BORDER_THICKNESS);
    gResetButtonGeometry->h = (signed) (RESET_BUTTON_HEIGHT / RESET_BUTTON_BORDER_THICKNESS);
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

SDL_Texture* makeTextTexture(char* text) {
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text, *gTextColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

char* numToText(int num) {
    char* text = SDL_calloc(MAX_NUM_LENGTH, sizeof(char));
    SDL_itoa(num, text, 10);
    return text;
}

void drawNum(SDL_Rect* rect, int num) {
    assert(num >= 0 && num <= MAX_NUM_VALUE);

    char* text = numToText(num);
    SDL_Texture* texture = makeTextTexture(text);
    SDL_RenderCopy(gRenderer, texture, NULL, rect);

    SDL_DestroyTexture(texture);
    SDL_free(text);
}

int calcFieldNumCoord(unsigned logicalCoord)
{ return (signed) ((logicalCoord * gTileSize / THICKNESS + gFieldStart) * THICKNESS); }

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

    for (unsigned row = 0, column; row < ROWS; row++) {
        for (column = 0; column < COLUMNS; column++) {
            rect.x = calcFieldNumCoord(row) + (signed) THICKNESS;
            rect.y = calcFieldNumCoord(column);
            drawNum(&rect, (signed) gFieldItems[row * COLUMNS + column]);
        }
    }
}

void drawCurrentScore() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize + THICKNESS),
        (signed) THICKNESS,
        175, 30
    };

    const unsigned scoreMsgLength = 15, maxLength = scoreMsgLength + MAX_NUM_LENGTH + 1;
    char* scoreText = numToText((signed) gScore);
    char* buffer = SDL_calloc(sizeof(char), maxLength);

    SDL_strlcat(buffer, "Current score: ", maxLength);
    SDL_strlcat(buffer, scoreText, maxLength);

    SDL_Texture* texture = makeTextTexture(buffer);
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

    SDL_Texture* texture = makeTextTexture("Reset");
    SDL_RenderCopy(gRenderer, texture, NULL, rect);
    SDL_DestroyTexture(texture);
    SDL_free(rect);
}

void rendererDraw() {
    setDrawColorToDefault();
    SDL_RenderClear(gRenderer);

    drawWindowFrame();
    drawField();
    drawCurrentScore();
    drawResetButton();

    SDL_RenderPresent(gRenderer);
}

void rendererClean() {
    SDL_free(gFieldItems);
    TTF_CloseFont(gFont);
    SDL_free(gTextColor);
    SDL_free(gResetButtonGeometry);
    TTF_Quit();
}

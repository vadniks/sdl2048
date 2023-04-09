
#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4, FIELD_ITEMS = ROWS * COLUMNS;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

unsigned* gFieldItems = NULL;
unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0, gFieldEnd = 0;
TTF_Font* gFont = NULL;
SDL_Color* gTextColor = NULL;
SDL_Renderer* gRenderer = NULL;

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

    gFieldItems = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    TTF_Init();
    gFont = TTF_OpenFont(FONT_PATH, 14);

    gTextColor = SDL_calloc(1, sizeof *gTextColor);
    gTextColor->a = 255;
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

SDL_Texture* makeTextTexture(const char* text) {
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, text, *gTextColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(gRenderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void drawField() {
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderSetScale(gRenderer, (float) THICKNESS, (float) THICKNESS);

    for (unsigned i = 0; i < ROWS + 1; i++) {
        const int each = (signed) (i * gTileSize / THICKNESS + gFieldStart);
        SDL_RenderDrawLine(gRenderer, each, (signed) gFieldStart, each, (signed) gFieldEnd);
        SDL_RenderDrawLine(gRenderer, (signed) gFieldStart, each, (signed) gFieldEnd, each);
    }


//    for (unsigned i = 0; i < FIELD_ITEMS; i++) {
//
//    }
}

void drawInfo() {
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize + THICKNESS),
        (signed) THICKNESS,
        120, 20
    };

    SDL_Texture* texture = makeTextTexture("Current score: ");
    SDL_RenderSetScale(gRenderer, 1, 1);
    SDL_RenderCopy(gRenderer, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
}

void rendererDraw() {
    setDrawColorToDefault();
    SDL_RenderClear(gRenderer);

    drawWindowFrame();
    drawField();
    drawInfo();

    SDL_RenderPresent(gRenderer);
}

void rendererClean() {
    SDL_free(gFieldItems);
    TTF_CloseFont(gFont);
    SDL_free(gTextColor);
    TTF_Quit();
}


#include <sdl_ttf/SDL_ttf.h>
#include <assert.h>
#include "render.h"

const unsigned ROWS = 4, COLUMNS = ROWS, THICKNESS = 4;
const char* FONT_PATH = "assets/Roboto-Regular.ttf";

unsigned* gField = NULL;
unsigned gWidth = 0, gHeight = 0, gFieldSize = 0, gTileSize = 0, gFieldStart = 0, gFieldEnd = 0;
TTF_Font* gFont = NULL;
SDL_Color* gTextColor = NULL;

void setDrawColorToDefault(SDL_Renderer* renderer)
{ SDL_SetRenderDrawColor(renderer, 49, 54, 59, 255); }

void rendererInit(SDL_Renderer* renderer) {
    assert(ROWS == COLUMNS);
    SDL_GetRendererOutputSize(renderer, (signed*) &gWidth, (signed*) &gHeight);

    gFieldSize = gWidth / 2;
    gTileSize = (gFieldSize - (ROWS + 1) * THICKNESS) / ROWS;
    gFieldStart = (signed) THICKNESS / 2;
    gFieldEnd = (signed) (ROWS * gTileSize / THICKNESS + gFieldStart);

    gField = SDL_malloc(sizeof(unsigned) * ROWS * COLUMNS);

    TTF_Init();
    gFont = TTF_OpenFont(FONT_PATH, 14);

    gTextColor = SDL_calloc(1, sizeof *gTextColor);
    gTextColor->a = 255;
}

void drawWindowFrame(SDL_Renderer* renderer) {
    SDL_Rect rect = (SDL_Rect) {
        0, 0,
        (signed) (gWidth / THICKNESS),
        (signed) (gHeight / THICKNESS)
    };

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float) THICKNESS, (float) THICKNESS);
    SDL_RenderDrawRect(renderer, &rect);
}

void drawField(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderSetScale(renderer, (float) THICKNESS, (float) THICKNESS);

    for (unsigned i = 0; i < ROWS + 1; i++) {
        const int each = (signed) (i * gTileSize / THICKNESS + gFieldStart);
        SDL_RenderDrawLine(renderer, each, (signed) gFieldStart, each, (signed) gFieldEnd);
        SDL_RenderDrawLine(renderer, (signed) gFieldStart, each, (signed) gFieldEnd, each);
    }
}

void drawInfo(SDL_Renderer* renderer) {
    SDL_Surface* surface = TTF_RenderText_Solid(gFont, "Current score: ", *gTextColor); // TODO: show score here
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_RenderSetScale(renderer, 1, 1);
    SDL_Rect rect = (SDL_Rect) {
        (signed) (gFieldSize + THICKNESS),
        (signed) THICKNESS,
        120, 20
    };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void rendererDraw(SDL_Renderer* renderer) {
    setDrawColorToDefault(renderer);
    SDL_RenderClear(renderer);

    drawWindowFrame(renderer);
    drawField(renderer);
    drawInfo(renderer);

    SDL_RenderPresent(renderer);
}

void rendererClean() {
    SDL_free(gField);
    TTF_CloseFont(gFont);
    SDL_free(gTextColor);
    TTF_Quit();
}

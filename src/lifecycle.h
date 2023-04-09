
#pragma once

#include <stdbool.h>

extern const unsigned WIDTH;
extern const unsigned HEIGHT;

bool gameInit();
void gameHandleEvents();
void gameUpdate();
void gameRender();
void gameClean();
bool gameLoop();

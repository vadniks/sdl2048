
#pragma once

#include <stdbool.h>

extern const unsigned WIDTH, HEIGHT;

bool lifecycleInit();
void lifecycleHandleEvents();
void lifecycleRender();
void lifecycleClean();
bool lifecycleLoop();

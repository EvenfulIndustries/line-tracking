#pragma once

#include "raylib.h"
#include <stdint.h>

#define  WIDTH 640
#define  HEIGHT 480
#define  RESOLUTION WIDTH * HEIGHT

Texture2D LoadTextureFromArray(uint8_t *eventBuffer, uint32_t bufferWidth, uint32_t bufferHeight);
#pragma once

#include "raylib.h"
#include <stdint.h>

Texture2D LoadTextureFromArray(uint8_t *eventBuffer, uint32_t bufferWidth, uint32_t bufferHeight);
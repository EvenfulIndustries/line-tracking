
#include <stdint.h>

#include "graphics.h"
#include "raylib.h"


Texture2D LoadTextureFromArray(uint8_t *eventBuffer, uint32_t bufferWidth, uint32_t bufferHeight) {
    // Create an image from the event buffer
    Image image = {0};
    image.data = eventBuffer;
    image.width = bufferWidth;
    image.height = bufferHeight;
    image.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
    image.mipmaps = 1;

    // Load the texture from the image
    Texture2D texture = LoadTextureFromImage(image);
    
    return texture;
}
#include <stdio.h>
#include <stdint.h>

#include "raylib.h"
#include "graphics.h"

#define EVENTFILE "/home/karp/CodeSpaces/personal/eventfulindustries/line-tracker/include/test.h" // Change this to your event file path
#include "linetracking.h"

#define LINEWIDTH 16

int main(){

    printf("Event count: %lu\n", event_count);

    InitWindow(640, 480, "event vision");

    uint8_t eventBuffer[640 * 480];
    SetTargetFPS(60);

    for(int i = 0; i + LINEWIDTH - 1 < 640 * 480; i += 2 * LINEWIDTH){
        
        for(int j = 0; j < LINEWIDTH; j++){
            eventBuffer[i + j] = 0xFF;
        }
        
    }

    while(!WindowShouldClose()) {

        Texture2D texture = LoadTextureFromArray(eventBuffer, 640, 480);

        BeginDrawing();
        DrawTexture(texture, 0, 0, WHITE);
        EndDrawing();

        UnloadTexture(texture);
    }

    CloseWindow();
    return 0;
}
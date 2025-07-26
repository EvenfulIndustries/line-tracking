#include <bits/time.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "raylib.h"
#include "graphics.h"

//#define EVENTFILE "/home/oskar/projects/thesis/line-tracking/data/extracted.h" // Change this to your event file path
#include "linetracking.h"

#define LINEWIDTH 16
#define COLLECTIONTIME 15 //The time the program collects events to be visualized in milliseconds

long diff_timespec_milliseconds(struct timespec t1, struct timespec t2);

int main(){

    printf("Event count: %lu\n", event_count);

    InitWindow(WIDTH, HEIGHT, "event vision");

    uint8_t event_collection_buffer[RESOLUTION];
    SetTargetFPS(60);

    //for(int i = 0; i + LINEWIDTH - 1 < RESOLUTION; i += 2 * LINEWIDTH){
        
    //    for(int j = 0; j < LINEWIDTH; j++){
    //        eventBuffer[i + j] = 0xFFFF;
    //    }
        
    //}


    struct timespec start, stop;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    stop = start;

    size_t current_event_index = 0;
    
    while(!WindowShouldClose() && current_event_index < event_count) {
        
        clock_gettime(CLOCK_MONOTONIC_RAW, &stop);
        if (diff_timespec_milliseconds(stop, start) > COLLECTIONTIME){

            uint32_t start_event_time = event_buffer[current_event_index].timestamp;
            while(current_event_index < event_count && event_buffer[current_event_index].timestamp - start_event_time <= COLLECTIONTIME * 1000) {
                //Collect stuff to eventBuffer
                event_t event = event_buffer[current_event_index];
                event_collection_buffer[event.x + event.y * WIDTH] += 20;
                current_event_index++;
            }

            Texture2D texture = LoadTextureFromArray(event_collection_buffer, WIDTH, HEIGHT);
            BeginDrawing();
            DrawTexture(texture, 0, 0, WHITE);
            EndDrawing();

            UnloadTexture(texture);
            memset(event_collection_buffer, 0, RESOLUTION);

            clock_gettime(CLOCK_MONOTONIC_RAW, &start);
        }
        
        
        
    }

    CloseWindow();
    return 0;
}


long diff_timespec_milliseconds(struct timespec t1, struct timespec t2) {
    return (t1.tv_sec - t2.tv_sec) * 1000 + (t1.tv_nsec - t2.tv_nsec) / 1000000;
}


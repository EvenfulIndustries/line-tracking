#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct event {
    uint32_t timestamp;  // Timestamp of the event in microseconds
    uint16_t x;          
    uint16_t y;          
} event_t;


event_t event_buffer[];
size_t event_count;  // Width of the event buffer

#ifdef EVENTFILE

#include EVENTFILE

#else

event_t event_buffer[] = {};
size_t event_count = 0;

#endif
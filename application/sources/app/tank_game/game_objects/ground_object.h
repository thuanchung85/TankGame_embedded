#ifndef __GROUND_OBJECT_H__
#define __GROUND_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"

// Forward declaration: Informs the compiler of AK's message structure without including it.
struct ak_msg_t;

// Defines signals specifically for the ground.
enum {
    GROUND_SETUP_SIG = 1,  // when Initial configuration for the ground
    GROUND_UPDATE_SIG,     // when Ground movement (scroll) command
    GROUND_RESET_SIG       // when Reset your state when you restart the game.
};

// Data structure for storing ground state
typedef struct {
    int8_t x;  // X-coordinates to create the scrolling effect
} ground_t;

// Declare extern so that the screen file (scr_game.cpp) can read the X-coordinates to draw.
extern ground_t static_ground;

#ifdef __cplusplus
extern "C" {
#endif

// Entry Point (Event Handling) function of Ground Task
void task_ground_handle(struct ak_msg_t *msg);

// Function to draw a bitmap of the ground
void ground_draw();

#ifdef __cplusplus
}
#endif

#endif // __GROUND_OBJECT_H__
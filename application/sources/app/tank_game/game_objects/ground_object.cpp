#include "ground_object.h"

ground_t static_ground = { .x = 0 };

void task_ground_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case GROUND_SETUP_SIG:
        case GROUND_RESET_SIG: {
            static_ground.x = 0; 
        } break;

        case GROUND_UPDATE_SIG: {
            static_ground.x++; 
            if (static_ground.x >= 24) {
                static_ground.x = 0;
            }
        } break;

        default:
            break;
    }
}

void ground_draw() {
    for (short x = 0; x < 124; x += 24) { 
        view_render.drawBitmap(x - (static_ground.x % 24), 52, bitmap_ground, 24, 8, WHITE);
    }
}
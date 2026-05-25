#include "mountain_object.h"

mountain_t static_mountain = {
    .x = 200,        // Đặt sẵn nó ở ngoài màn hình bên phải
    .frame_count = 0
};

void task_mountain_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case MOUNTAIN_SETUP_SIG:
        case MOUNTAIN_RESET_SIG: {
            static_mountain.x = 200; 
        } break;

        case MOUNTAIN_UPDATE_SIG: {
            static_mountain.frame_count++;
            if(static_mountain.frame_count >= 6)
            {
                static_mountain.x--;
                if (static_mountain.x < -120) 
                {
                    static_mountain.x = 200 + 90; 
                }
                static_mountain.frame_count = 0;
            }
        } break;

        default:
            break;
    }
}

void mountain_draw() {
    view_render.drawBitmap(static_mountain.x, 0, bitmap_theMountain, 90, 42, WHITE);
}
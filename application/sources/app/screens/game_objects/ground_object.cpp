#include "ground_object.h"

// Khởi tạo vùng nhớ tĩnh toàn cục cho Mặt đất
ground_t static_ground;

void task_ground_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        //setup và reset mặt đất x về 0
        case GROUND_SETUP_SIG:
        case GROUND_RESET_SIG: {
            static_ground.x = 0; 
        } break;

        //nếu nhận tính hiệu GROUND_UPDATE_SIG thì cuộn x
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

//vẽ ground bitmap scrolling theo x
void ground_draw() {
    for (short x = 0; x < 124; x += 24) { 
        view_render.drawBitmap(x - (static_ground.x % 24), 52, bitmap_ground, 24, 8, WHITE);
    }
}
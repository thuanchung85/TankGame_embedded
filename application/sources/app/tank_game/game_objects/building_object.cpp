#include "building_object.h"

building_t static_building = {
    .x = 180,        // Đặt sẵn nó ở ngoài màn hình bên phải
    .building_type = 0,   // Loại nhà đầu tiên
    .frame_count = 0
};

const unsigned char* building_bitmaps[] = {
        bitmap_house1, 
        bitmap_house2, 
        bitmap_house3, 
        bitmap_house4
    };

void task_building_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case BUILDING_SETUP_SIG:
        case BUILDING_RESET_SIG: {
            static_building.x = 180; 
            static_building.building_type = 0;
        } break;

        case BUILDING_UPDATE_SIG: {
            static_building.frame_count++;
            if(static_building.frame_count >= 2)
            {
                static_building.x--;
                if (static_building.x < -120) 
                {
                    static_building.x = 130 + (rand() % 100); 
                    static_building.building_type = rand() % 4;
                }
                static_building.frame_count = 0;
            }
        } break;

        default:
            break;
    }
}

void building_draw() {
    view_render.drawBitmap(static_building.x, 10, building_bitmaps[static_building.building_type], 30, 30, WHITE);
}
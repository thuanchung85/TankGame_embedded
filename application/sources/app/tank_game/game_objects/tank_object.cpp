#include "tank_object.h"

tank_t static_tank = { 
    .x = 20,
    .target_x = 20,
    .frame_count = 0,
    .explosion_timer = 0,
    .is_cannon_reloaded = true,
    .isExploding = false,
    .isMoving = false,
    .is_firing_gun = false,
    .isDie = false
};

void task_tank_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case TANK_SETUP_SIG:
        case TANK_RESET_SIG: {
            static_tank.x = 20;
            static_tank.target_x = 20;
            static_tank.frame_count = 0;
            static_tank.explosion_timer = 0;
            static_tank.is_cannon_reloaded = true;
            static_tank.isExploding = false;
            static_tank.isMoving = false;
            static_tank.is_firing_gun = false;
            static_tank.isDie = false;
        } break;

        case TANK_UPDATE_SIG: {
           
            //tank die already
            if (static_tank.isDie) break;

            //tank on exploding
            if (static_tank.isExploding) {
                static_tank.explosion_timer++;
                
                if (static_tank.explosion_timer > 20) {
                    static_tank.isExploding = false;
                    static_tank.isDie = true; 
                }
            }
            // tank is ok
            else {
                if (static_tank.isMoving == true) {
                    if (static_tank.x < static_tank.target_x) static_tank.x += 1;
                    if (static_tank.x > static_tank.target_x) static_tank.x -= 1;
                    if (static_tank.x == static_tank.target_x) static_tank.isMoving = false;
                }
            }
        } break;


        case TANK_MOVE_FORWARD_SIG: {
            if (!static_tank.isExploding) {
                static_tank.target_x += 10;
                if (static_tank.target_x > 60) static_tank.target_x = 60;
                static_tank.isMoving = true;
            }
        } break;

        case TANK_MOVE_BACKWARD_SIG: {
            if (!static_tank.isExploding) {
                static_tank.target_x -= 10;
                if (static_tank.target_x < 5) static_tank.target_x = 5;
                static_tank.isMoving = true;
            }
        } break;

        default:
            break;
    }
}

void tank_draw() {
   // 1. Trường hợp xe tăng đã chết hẳn -> Không vẽ gì cả (hoặc vẽ chữ GAME OVER)
    if (static_tank.isDie) {
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(35, 25);
        view_render.print("GAME OVER");
        return;
    }

    // 2. Trường hợp xe tăng đang nổ -> Vẽ tuần tự các bước animation
    if (static_tank.isExploding) {
        if (static_tank.explosion_timer < 6) {
            view_render.drawBitmap(static_tank.x, 30, bitmap_bum, 28, 20, WHITE);
        }
        else if (static_tank.explosion_timer < 12) {
            view_render.drawBitmap(static_tank.x, 30, bitmap_bum2, 30, 26, WHITE);
        }
        else {
            view_render.drawBitmap(static_tank.x, 30, bitmap_bum3, 30, 26, WHITE);
        }
    }
    // 3. Trường hợp xe tăng bình thường -> Vẽ xe tăng
    else {
        view_render.drawBitmap(static_tank.x, 30, bitmap_tank, 30, 30, WHITE);
    }
}
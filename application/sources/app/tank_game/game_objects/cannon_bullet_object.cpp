#include "cannon_bullet_object.h"

cannon_bullet_t static_cannon_bullet = { 
    .x = 0,
    .y = 0,
    .is_active = false
};

void task_cannon_bullet_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case CANNON_BULLET_SETUP_SIG:
        case CANNON_BULLET_RESET_SIG: {
            static_cannon_bullet.x = 0;
            static_cannon_bullet.y = 0;
            static_cannon_bullet.is_active = false;
        } break;

        case CANNON_BULLET_FIRE_SIG: {
            if (!static_cannon_bullet.is_active) {
                extern tank_t static_tank; 
                static_cannon_bullet.x = static_tank.x + 28; 
                static_cannon_bullet.y = 39; 
                static_cannon_bullet.is_active = true;
                BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            }
        } break;

        case CANNON_BULLET_UPDATE_SIG: {
            if (static_cannon_bullet.is_active) {
                static_cannon_bullet.x += 2; 

                if (static_cannon_bullet.x > 128) {
                    static_cannon_bullet.is_active = false;
                    static_cannon_bullet.x = 0; 
                }
            }
        } break;


        default:
            break;
    }
}

void cannon_bullet_draw() {
    if (static_cannon_bullet.is_active) {
        view_render.drawBitmap(static_cannon_bullet.x, static_cannon_bullet.y, bitmap_myTank_cannon_bullet, 5, 3, WHITE);
    }
}
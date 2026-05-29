#include "trap_object.h"

trap_t static_trap = {
   .x = 0,
    .y = -20,        
    .is_active = false,
    .isExploding = false,
    .isDestroy = false,
    .explosionTimer = 0
};
static int16_t trap_spawn_cooldown = 0;

// fuction helper reset rocket
static void trap_internal_reset() {
    static_trap.x = rand() % 100;    
    static_trap.y = -20;             
    static_trap.isExploding = false;
    static_trap.isDestroy = false;
    static_trap.explosionTimer = 0;
    trap_spawn_cooldown = 50 + (rand() % 100);
}

//function check hit with minigun bullet
static void check_collision_minigun_with_trap() {
    if (!static_trap.is_active || static_trap.isExploding || static_trap.isDestroy) {
        return;
    }

    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
        if (minigun_pool[i].is_active) {
            if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                static_trap.x, static_trap.y, 11, 17)) 
            {
                minigun_pool[i].is_active = false; 
                static_trap.isExploding = true;
                static_trap.explosionTimer = 0;
                
                BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                APP_DBG(">> TRAP DESTROYED BY MINIGUN! <<\n");
                
                break; 
            }
        }
    }
}



void task_trap_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case TRAP_SETUP_SIG:
        case TRAP_RESET_SIG: {
            trap_internal_reset();
            static_trap.is_active = true; 
            APP_DBG(">> TRAP SYSTEM INITIALIZED! <<\n");
        } break;

     case TRAP_UPDATE_SIG: {
            if (!static_trap.is_active) break;
            if (trap_spawn_cooldown > 0) {
                trap_spawn_cooldown--;
                break; 
            }

            if (static_trap.isExploding) {
                static_trap.explosionTimer++;
                if (static_trap.explosionTimer > 12) { 
                    static_trap.isExploding = false;
                    static_trap.isDestroy = true;
                    trap_internal_reset();             
                }
                break; 
            }

           static_trap.y += 1; 

            if (static_trap.y > 65) {
                trap_internal_reset(); 
                break; 
            }

            check_collision_minigun_with_trap();

            if (!static_tank.isExploding && !static_tank.isDie) {
                if (check_collision(static_trap.x, static_trap.y, 5, 8,
                                    static_tank.x, 30, 25, 15)) 
                {
                    static_tank.isExploding = true;
                    static_tank.explosion_timer = 0;
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
                    APP_DBG(">> TANK CRASHED BY TRAP ROCKET! <<\n");

                    trap_internal_reset();
                }
            }
        } break;

        default:
            break;
    }
}

void trap_draw() {
    if (!static_trap.is_active) return;

    if (static_trap.isExploding) {
        if (static_trap.explosionTimer < 4) {
            view_render.drawBitmap(static_trap.x, static_trap.y, bitmap_bum, 28, 20, WHITE);
        } else if (static_trap.explosionTimer < 8) {
            view_render.drawBitmap(static_trap.x, static_trap.y, bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(static_trap.x, static_trap.y, bitmap_bum3, 30, 31, WHITE);
        }
    }
    else if (!static_trap.isDestroy) {
        view_render.drawBitmap(static_trap.x, static_trap.y, bitmap_enemy_rocket, 11, 17, WHITE);
    }
}
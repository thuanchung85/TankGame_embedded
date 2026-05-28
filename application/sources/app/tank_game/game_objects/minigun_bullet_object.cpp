#include "minigun_bullet_object.h"

 minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];

void task_minigun_bullet_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case MINIGUN_BULLET_SETUP_SIG:
        case MINIGUN_BULLET_RESET_SIG: {
            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                minigun_pool[i].is_active = false;
                minigun_pool[i].x = 0;
                minigun_pool[i].y = 0;
                minigun_pool[i].targetY = 0;
            }
        } break;

      case MINIGUN_BULLET_FIRE_SIG: {
            extern tank_t static_tank;
            extern enemy_t static_enemy;
            extern boss_t static_boss; 

            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                if (!minigun_pool[i].is_active) { 
                    minigun_pool[i].x = static_tank.x + 18; 
                    minigun_pool[i].y = 31;                

                    if (static_boss.is_active && static_boss.rocket.is_active) {
                        minigun_pool[i].targetY = static_boss.rocket.y + 5; 
                    }
                    
                    else if (static_boss2.is_active && static_boss2.cannon_bullet.is_active) {
                        minigun_pool[i].targetY = static_boss2.cannon_bullet.y + 5; 
                    }

                    else if (static_boss.is_active && !static_boss.is_exploding) {
                        minigun_pool[i].targetY = static_boss.y + 18;
                    }
                    else if (static_enemy.enemy_type == 1 && !static_enemy.isExploding) {
                        minigun_pool[i].targetY = 5; 
                    } 
                    else if (static_trap.is_active  && !static_trap.isExploding) {
                        minigun_pool[i].targetY = static_trap.y + 5; 
                    }
                    else {
                        minigun_pool[i].targetY = -10; 
                    }

                    minigun_pool[i].is_active = true;
                    break; 
                }
            }
        } break;

        case MINIGUN_BULLET_UPDATE_SIG: {
            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                if (minigun_pool[i].is_active) {
                    minigun_pool[i].x += 2; 

                    if (minigun_pool[i].y > minigun_pool[i].targetY) {
                        minigun_pool[i].y -= 1; 
                    } 
                    else if (minigun_pool[i].y < minigun_pool[i].targetY) {
                        minigun_pool[i].y += 1; 
                    }

                    if (minigun_pool[i].x >= 124 || minigun_pool[i].y < 0 || minigun_pool[i].y > 60) {
                        minigun_pool[i].is_active = false;
                    }
                }
            }
        } break;

        default:
            break;
    }
}

void minigun_bullet_draw() {
    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
        if (minigun_pool[i].is_active) {
            view_render.drawBitmap(minigun_pool[i].x, minigun_pool[i].y, bitmap_myTank_gun_bullet, 2, 1, WHITE);
        }
    }
}
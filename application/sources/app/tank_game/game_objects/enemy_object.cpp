#include "enemy_object.h"

enemy_t static_enemy = { 
    .isExploding = false,    
    .enemy_type = 0,
    .hp = 1,              
    .explosionTimer = 0,  
    .x = 150
};

static void enemy_reset(){
    static_enemy.isExploding = false;
    static_enemy.explosionTimer = 0;
    static_enemy.enemy_type = rand() % 4;
    if (static_enemy.enemy_type == 0 || static_enemy.enemy_type == 1) { 
        static_enemy.hp = 2;
    } else {               
        static_enemy.hp = 1;
    }
    static_enemy.x = 150 + (rand() % 100);
}

void task_enemy_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case ENEMY_SETUP_SIG:
        case ENEMY_RESET_SIG: {
           enemy_reset();
        } break;

        case ENEMY_UPDATE_SIG: {
            if (static_enemy.isExploding) 
            {
                static_enemy.explosionTimer++;
                if (static_enemy.explosionTimer > 9) {
                    enemy_reset();
                }
                return;
            }
            else 
            {
                if(static_enemy.enemy_type == 3 || static_enemy.enemy_type == 1)
                {
                    static_enemy.x -= 2;
                }
                else
                {
                    static_enemy.x--;
                }
                if (static_enemy.x < -35) {
                    enemy_reset();
                }
            }   
        
        } break;

        default:
            break;
    }
}

void enemy_draw() {
    if (static_enemy.isExploding) {
        if(static_enemy.enemy_type != 1)//for tank, mine, troop at y = 30
        {
            if (static_enemy.explosionTimer < 3) {
                view_render.drawBitmap(static_enemy.x, 30, bitmap_bum, 28, 20, WHITE);
            } else if (static_enemy.explosionTimer < 6) {
                view_render.drawBitmap(static_enemy.x, 25, bitmap_bum2, 30, 26, WHITE);
            } else {
                view_render.drawBitmap(static_enemy.x, 20, bitmap_bum3, 30, 31, WHITE);
            }
        }
        else //for air at y = 5
        {
            if (static_enemy.explosionTimer < 3) {
                view_render.drawBitmap(static_enemy.x, 5, bitmap_bum, 28, 20, WHITE);
            } else if (static_enemy.explosionTimer < 6) {
                view_render.drawBitmap(static_enemy.x, 5, bitmap_bum2, 30, 26, WHITE);
            } else {
                view_render.drawBitmap(static_enemy.x, 5, bitmap_bum3, 30, 31, WHITE);
            }
        }

    } 
    else 
    {
        switch (static_enemy.enemy_type)
        {
            case 0:// tank
                    view_render.drawBitmap(static_enemy.x, 33, bitmap_enemy_tank, 25, 21, WHITE);
                    break;

            case 1:// air
                    view_render.drawBitmap(static_enemy.x, 5, bitmap_enemy_air_plane, 25, 21, WHITE);
                    break;

            case 2:// mine
                    view_render.drawBitmap(static_enemy.x, 33, bitmap_enemy_mine, 22, 21, WHITE);
                    break;

            case 3:// troop
                view_render.drawBitmap(static_enemy.x, 34, bitmap_enemy_troop, 15, 21, WHITE);
                break;
        }
    }
}
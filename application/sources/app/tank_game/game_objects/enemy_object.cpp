#include "enemy_object.h"

enemy_t static_enemy={
    .isExploding = false,    
    .enemy_type = 0,
    .hp = 1,              
    .explosionTimer = 0,  
    .x = 150
};

// function check collision cannon bullet with enemy
 void check_collision_cannon_bullet_with_enemy()
{
    if (static_cannon_bullet.is_active && !static_enemy.isExploding)
    {
        // get (Bounding Box) of enemy type
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;

        if (static_enemy.enemy_type == 1)
            enemy_y = 5; // for air plane
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        } // for troop

        // Check cannon bullet (5x3) hit test enemy
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_enemy.x, enemy_y, enemy_w, enemy_h))
        {
            // deactive cannon bullet
            static_cannon_bullet.is_active = false;

            // enemy loss hp
            if (static_enemy.hp > 0)
                static_enemy.hp--;

            // if no hp then enemy destroy
            if (static_enemy.hp == 0)
            {
                static_enemy.isExploding = true;
                static_enemy.explosionTimer = 0;
                // APP_DBG(">> ENEMY DESTROYED!\n");
                add_score();
            }
            else
            {
                // APP_DBG(">> ENEMY HIT! HP REMAINING: %d\n", static_enemy.hp);
            }
        }
    }
}

// function check collision minigun bullet with enemy
 void check_collision_minigun_bullet_with_enemy()
{
    if (!static_enemy.isExploding)
    {
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
        if (static_enemy.enemy_type == 1)
            enemy_y = 5;
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        }

        // loop for array of minigun bullet
        for (int i = 0; i < MAX_MINIGUN_BULLETS; i++)
        {
            if (minigun_pool[i].is_active)
            {
                if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                    static_enemy.x, enemy_y, enemy_w, enemy_h))
                {
                    minigun_pool[i].is_active = false; // deactive the bullet hit enemy

                    if (static_enemy.hp > 0)
                        static_enemy.hp--;

                    if (static_enemy.hp == 0)
                    {
                        static_enemy.isExploding = true;
                        static_enemy.explosionTimer = 0;

                        // add score
                        add_score();
                    }
                    break;
                }
            }
        }
    }
}

// function check collision tank with enemy
 void check_collision_tank_with_enemy()
{
    if (!static_tank.isExploding && !static_enemy.isExploding)
    {
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
        if (static_enemy.enemy_type == 1)
            enemy_y = 5;
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        }

        // Check my tank hit with enemy (30x30 ,at Y=30)
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_enemy.x, enemy_y, enemy_w, enemy_h))
        {
            // tank destroy
            static_tank.isExploding = true;
            static_tank.explosion_timer = 0;
            // APP_DBG(">> GAME OVER - TANK CRASHED!\n");
            static_enemy.isExploding = true;
            static_enemy.explosionTimer = 0;
            // APP_DBG(">> ENEMY DESTROYED!\n");
        }
    }
}


 void enemy_reset(){
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
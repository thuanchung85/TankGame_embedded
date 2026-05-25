#include "tank_object.h"

tank_t static_tank = { 
    .x = 20,
    .target_x = 20,
    .frame_count = 0,
    .explosion_timer = 0,
    .is_cannon_reloaded = true,
    .isExploding = false,
    .isMoving = false,
    .is_firing_gun = false
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
        } break;

        case TANK_UPDATE_SIG: {
            //if tank alive
            if(!static_tank.isExploding)
            { 
                //tank move to x target
                if(static_tank.isMoving == true)
                {
                    if (static_tank.x < static_tank.target_x) static_tank.x += 1;
                    if (static_tank.x > static_tank.target_x) static_tank.x -= 1;
                    if(static_tank.x == static_tank.target_x) static_tank.isMoving = false;
                }
            
            }
            else// if tank die
            {
                static_tank.explosion_timer++;
                if(static_tank.explosion_timer > 20) { 
                    static_tank.explosion_timer = 0;
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
   //if tank die
    if (static_tank.isExploding) 
    {
        // animation bitmap (bum, bum2, bum3)
        if (static_tank.explosion_timer < 5) view_render.drawBitmap(static_tank.x, 30, bitmap_bum, 28, 20, WHITE);
        else if (static_tank.explosion_timer < 10) view_render.drawBitmap(static_tank.x, 30, bitmap_bum2, 30, 26, WHITE);
         else if (static_tank.explosion_timer < 15) view_render.drawBitmap(static_tank.x, 30, bitmap_bum3, 30, 26, WHITE);
    }
    //if tank alive
    else
    {
        view_render.drawBitmap(static_tank.x, 30, bitmap_tank, 30, 30, WHITE);//draw tank

    }
}
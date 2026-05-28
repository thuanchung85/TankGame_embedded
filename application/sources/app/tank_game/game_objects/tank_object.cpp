#include "tank_object.h"

static uint8_t gameover_timer_countdown = 0;

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

            gameover_timer_countdown = 0;//for gameover screen reset
        } break;

        case TANK_UPDATE_SIG: {
           
            //tank die already
            if (static_tank.isDie) {
                gameover_timer_countdown++;
                if (gameover_timer_countdown >= 40) { 
                    gameover_timer_countdown = 0;
                    
                    APP_DBG(">> TIMEOUT FINISHED -> CLEAN UP GAME OBJECTS... <<\n");
                    
                    // =========================================================
                    //ALL RESET GAME OBJECT 
                    // =========================================================
                    static_boss.is_active = false;
                    static_boss.is_exploding = false;
                    static_boss.isDie = true;
                    static_boss.rocket.is_active = false;
                    static_boss2.is_active = false;
                    static_boss2.is_exploding = false;
                    static_boss2.isDie = true;
                    static_trap.is_active = false;
                    static_trap.isExploding = false;
                    static_trap.isDestroy = true;

                    task_post_pure_msg(TG_ENEMY_TASK_ID, ENEMY_RESET_SIG);
                    task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_RESET_SIG);
                    task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_RESET_SIG); 
                    task_post_pure_msg(TG_TRAP_TASK_ID, TRAP_RESET_SIG);
                    task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_RESET_SIG);
                    task_post_pure_msg(TG_TANK_TASK_ID, TANK_RESET_SIG);
                    task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_RESET_SIG);
                    task_post_pure_msg(TG_TREE_TASK_ID, TREE_RESET_SIG);
                    task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_RESET_SIG);
                    task_post_pure_msg(TG_SCORE_TASK_ID, SCORE_RESET_SIG);


                    APP_DBG(">> GO TO GAME OVER SCREEN! <<\n");
                    SCREEN_TRAN(scr_game_over_handle, &scr_game_over);
                }
                break; 
            }

            //tank on exploding
            if (static_tank.isExploding) {
                static_tank.explosion_timer++;
                
                if (static_tank.explosion_timer > 20) {
                    static_tank.isExploding = false;
                    static_tank.isDie = true; 
                    gameover_timer_countdown = 0;
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
                if (static_tank.target_x < 0) static_tank.target_x = 0;
                static_tank.isMoving = true;
            }
        } break;

        default:
            break;
    }
}

void tank_draw() {
    // 1. If the tank is completely dead -> Draw nothing (or draw the words GAME OVER)
    if (static_tank.isDie) {
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(5, 10);
        view_render.print("YOUR TANK IS DESTROY");
        return;
    }

    // 2. Case where the tank is exploding -> Draw the animation steps sequentially
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
    
    // 3. Normal tank case -> Draw the tank
    else {
        view_render.drawBitmap(static_tank.x, 30, bitmap_tank, 30, 30, WHITE);
    }
}
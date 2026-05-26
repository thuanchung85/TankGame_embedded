#include "scr_game.h"

//  ================  VARIABLES  ================ //
static bool is_minigun_firing = false; //for active minigun fire

// variable access tank, cannon bullet...enemies
extern tank_t static_tank;
extern cannon_bullet_t static_cannon_bullet;
extern enemy_t static_enemy;
extern minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];

//  ================  HELPER FUNCTIONS ================ //

// check if game objects rectangle over each other
static bool check_collision(int16_t x1, int16_t y1, int16_t w1, int16_t h1,
                            int16_t x2, int16_t y2, int16_t w2, int16_t h2) 
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

// function fire minigun
static void minigun_fire()
{
    if (is_minigun_firing)
    {
        static uint8_t minigun_fire_divider = 0; // bullets delay of minigun
        minigun_fire_divider++;
        if (minigun_fire_divider >= 8)
        { // delay 8 loop (120ms)
            task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_FIRE_SIG);
            minigun_fire_divider = 0;
        }
    }
}

//function check collision cannon bullet with enemy
static void check_collision_cannon_bullet_with_enemy()
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
            }
            else
            {
                // APP_DBG(">> ENEMY HIT! HP REMAINING: %d\n", static_enemy.hp);
            }
        }
    }
}

//function check collision minigun bullet with enemy
static void check_collision_minigun_bullet_with_enemy()
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
                    }
                    break; 
                }
            }
        }
    }
}

//function check collision tank with enemy
static void check_collision_tank_with_enemy()
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
            //tank destroy
            static_tank.isExploding = true;
            static_tank.explosion_timer = 0;
            // APP_DBG(">> GAME OVER - TANK CRASHED!\n");
            static_enemy.isExploding = true;
            static_enemy.explosionTimer = 0;
            // APP_DBG(">> ENEMY DESTROYED!\n");
        }
    }
}


//  ================  DRAW GAME OBJECTS ================ //
static void view_scr_game()
{      
    mountain_draw(); 
    building_draw(); 
    ground_draw();   
    tank_draw();     
    enemy_draw();   
    tree_draw();     
    cannon_bullet_draw(); 
    minigun_bullet_draw();
};

view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game 
};

view_screen_t scr_game = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// ================LOGIC (EVENT HANDLER)======================== //
void scr_game_handle(ak_msg_t* msg)
{
    switch (msg -> sig)
    {
        case SCREEN_ENTRY:
        {
            APP_DBG(">> Entered TANK Screen Success!\n");
            // setup for objects in game
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_SETUP_SIG);
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_SETUP_SIG); 
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_SETUP_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_SETUP_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_SETUP_SIG); 
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_SETUP_SIG); 


            // timer for game loop
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60, TIMER_PERIODIC);
        }
        break;

        // game loop
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: 
        {
            //APP_DBG(">>  TANK UPDATE!\n");
            // each signal AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, we post some task below
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_UPDATE_SIG);
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_UPDATE_SIG);
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_UPDATE_SIG);
            task_post_pure_msg(TG_ENEMY_TASK_ID, ENEMY_UPDATE_SIG); 

            task_post_pure_msg(TG_TREE_TASK_ID, TREE_UPDATE_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_UPDATE_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_UPDATE_SIG); 
            task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_UPDATE_SIG);

            minigun_fire();
            check_collision_cannon_bullet_with_enemy();
            check_collision_minigun_bullet_with_enemy();
            check_collision_tank_with_enemy();
            
        }
        break;

        //when MODE button down and HOLD
        case AC_DISPLAY_BUTON_MODE_DOWN:
            if(!static_tank.isExploding) is_minigun_firing = true;//active minigun
            break;
        case AC_DISPLAY_BUTTON_MODE_HOLD:
             if(!static_tank.isExploding) is_minigun_firing = true;//active minigun
            break;

        //button MODE released
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            is_minigun_firing = false; // stop minigun
             if(!static_tank.isExploding) task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_FIRE_SIG);//fire cannon
        break;

        //button UP released
        case AC_DISPLAY_BUTON_UP_RELEASED:
             if(!static_tank.isExploding) task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_FORWARD_SIG);//move tank forward
        break;

        //button DOWN released
        case AC_DISPLAY_BUTON_DOWN_RELEASED:
             if(!static_tank.isExploding) task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_BACKWARD_SIG);//move tank backward
        break;

        default:
            break;
    }
}
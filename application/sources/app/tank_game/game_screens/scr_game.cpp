#include "scr_game.h"

//  ================  VARIABLES  ================ //
static bool is_minigun_firing = false; // for active minigun fire

// variable access tank, cannon bullet...enemies
extern tank_t static_tank;
extern cannon_bullet_t static_cannon_bullet;
extern enemy_t static_enemy;
extern boss_t static_boss;
extern boss2_t static_boss2;
extern score_object_t static_score;
extern minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];
extern trap_t static_trap;

//  ================  HELPER FUNCTIONS ================ //

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


//  ================  DRAW GAME OBJECTS ================ //
static void view_scr_game()
{
    mountain_draw();
    building_draw();
    ground_draw();
    tank_draw();
    trap_draw();
    enemy_draw();
    tree_draw();
    cannon_bullet_draw();
    minigun_bullet_draw();
    score_draw();
    boss_draw();
    boss2_draw();
};

view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game};

view_screen_t scr_game = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

// ================LOGIC (EVENT HANDLER)======================== //
void scr_game_handle(ak_msg_t *msg)
{
    switch (msg->sig)
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
        task_post_pure_msg(TG_TRAP_TASK_ID, TRAP_SETUP_SIG);

        // --- RESET SCORE  ---
        task_post_pure_msg(TG_SCORE_TASK_ID, SCORE_SETUP_SIG);

        // --- RESET BOSS, BOSS2  ---
        task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_SETUP_SIG);
        task_post_pure_msg(TG_BOSS2_TASK_ID, BOSS2_SETUP_SIG);

        // timer for game loop
        timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60, TIMER_PERIODIC);
    }
    break;

    // game loop
    case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE:
    {

        // update ground
        task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_UPDATE_SIG);
        // update tank
        task_post_pure_msg(TG_TANK_TASK_ID, TANK_UPDATE_SIG);
        // update tank cannon bullet
        task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_UPDATE_SIG);

        // update enenmies
        task_post_pure_msg(TG_ENEMY_TASK_ID, ENEMY_UPDATE_SIG);
        // update trap
        task_post_pure_msg(TG_TRAP_TASK_ID, TRAP_UPDATE_SIG);

        // update Boss 1 
        if (static_boss.is_active || static_boss.is_exploding)
        {
            task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_UPDATE_SIG);
        }

        // update Boss 2 
        if (static_boss2.is_active || static_boss2.is_exploding)
        {
            task_post_pure_msg(TG_BOSS2_TASK_ID, BOSS_UPDATE_SIG);
        }

        task_post_pure_msg(TG_TREE_TASK_ID, TREE_UPDATE_SIG);
        task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_UPDATE_SIG);
        task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_UPDATE_SIG);
        task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_UPDATE_SIG);

        // fire minigun when hold mode button
        minigun_fire();

        // check collision between game objects when boss or boss2 not show
        if (!static_boss.is_active || !static_boss2.is_active)
        {
            check_collision_cannon_bullet_with_enemy();
            check_collision_minigun_bullet_with_enemy();
            check_collision_tank_with_enemy();
        }

        // BOSS call when 100 score
        check_and_spawn_boss(400);
        // BOSS2 call when 350 score
        check_and_spawn_boss2(10);
      
      }
    break;

    // when MODE button down and HOLD
    case AC_DISPLAY_BUTON_MODE_DOWN:
        if (!static_tank.isExploding && !static_tank.isDie)
            is_minigun_firing = true; // active minigun
        break;
    case AC_DISPLAY_BUTTON_MODE_HOLD:
        if (!static_tank.isExploding && !static_tank.isDie)
            is_minigun_firing = true; // active minigun
        break;

    // button MODE released
    case AC_DISPLAY_BUTON_MODE_RELEASED:
        is_minigun_firing = false; // stop minigun
        if (!static_tank.isExploding && !static_tank.isDie)
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_FIRE_SIG); // fire cannon
        break;

    // button UP released
    case AC_DISPLAY_BUTON_UP_RELEASED:
        if (!static_tank.isExploding && !static_tank.isDie)
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_FORWARD_SIG); // move tank forward
        break;

    // button DOWN released
    case AC_DISPLAY_BUTON_DOWN_RELEASED:
        if (!static_tank.isExploding && !static_tank.isDie)
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_BACKWARD_SIG); // move tank backward
        break;

    default:
        break;
    }
}
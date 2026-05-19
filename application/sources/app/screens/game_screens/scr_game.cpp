#include "scr_game.h"
#include "game_objects/Tank.h"
#include "game_objects/Ground.h"
#include "game_objects/Tree.h"
#include "game_objects/Mountain.h"
#include "game_objects/House.h"
#include "game_objects/Score.h"
#include "game_objects/Enemy.h"
#include "game_objects/Trap.h"
#include "game_objects/Boss.h" 
#include "game_managers/CollisionManager.h"
#include "game_managers/GameEventsManager.h"

//============game objects ========//
static Tank my_tank;// tank
static Ground my_ground;//scrolling ground
static Tree my_tree;//scrolling tree
static Mountain my_mountain;//scrolling mountain
static House my_house;//scrolling house
static Score my_score;//score
static Enemy my_enemy; //enemy
static Trap my_trap; //rocket drop
static Boss my_boss; //Boss
static GameEventsManager game_events;//manage game pause, game over, boss spawn,victory, reset, update save score

// function reset game
static void reset_game_bridge() {
    game_events.isGameOver = false;
    game_events.isPaused = false;
    game_events.isVictory = false;
    game_events.game_over_delay_counter = 0;
    my_tank.reset(); my_score.reset(); my_house.reset(); my_tree.reset();
    my_enemy.reset(); my_trap.reset(); my_mountain.reset(); my_boss.reset();
}


static void view_scr_game()
{    
    //draw mountain
    my_mountain.draw();
    //draw house
    my_house.draw();
    //draw tree
    my_tree.draw();
    //draw tank
    my_tank.draw();
    //draw enemy when boss not show
    if(!my_boss.is_active) my_enemy.draw();
    // draw Boss 
    my_boss.draw(); 
    //draw trap
    my_trap.draw();
    //draw ground 
    my_ground.draw();
    //draw score
    my_score.draw();

    //game_events
    if (game_events.isGameOver) 
    {
        view_render.fillRect(0,0,130,65,BLACK); 
        view_render.drawBitmap(0, 0, bitmap_game_over, 124, 60, WHITE);
        view_render.setTextSize(1); view_render.setCursor(90, 30); view_render.print(my_score.current_score);
    } 
    else if (game_events.isVictory)
    {
        view_render.fillRect(0,0,130,65,BLACK); 
        view_render.drawBitmap(0, 0, bitmap_victory, 124, 60, WHITE);
        view_render.setTextSize(1); view_render.setCursor(90, 30); view_render.print(my_score.current_score);
    }
    else if (game_events.isPaused) 
    {
        view_render.fillRect(0,0,130,65,BLACK); 
        view_render.drawBitmap(0, 0, bitmap_game_pause, 124, 60, WHITE);
    }
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


// ================logic (Event Handler)========================
void scr_game_handle(ak_msg_t* msg)
{
    switch (msg -> sig)
    {
        case SCREEN_ENTRY:
            APP_DBG(">> Entered TANK Screen Success!\n");
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
            reset_game_bridge(); //game reset
            //set timer update game loop each 60ms
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60,TIMER_PERIODIC);
            break;

        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: 
        {
            //game stop update when pause, game over, victory
            if (game_events.isGameOver || game_events.isPaused || game_events.isVictory) return;

            my_ground.update();//update scroliing ground
            my_tank.update(); //update tank

            // SPAWN BOSS when score at 200
           game_events.check_boss_spawn(my_score, my_boss);

            //update boss
            my_boss.update();  
            if(!my_boss.is_active) my_enemy.update();//update enemy tank 
            my_tree.update();//tree update
            my_trap.update();//update rocket trap
            my_house.update();// update scrolling house
            my_mountain.update();//update mountain
            my_score.update();//update score 
    
            // TANK auto gun update attack air, and boss rocket
            if (my_enemy.enemy_type == 1 && my_enemy.x < 120 && !my_boss.is_active) my_tank.tank_fire_gun(5); 
            else if (my_boss.rocket.is_active && my_boss.rocket.x < 120) {
                // try shooting Rocket Boss
                my_tank.tank_fire_gun(my_boss.rocket.y); 
            }

            // CHECK (COLLISION PHASE)!
            CollisionManager::check_all_collisions(my_tank, my_enemy, my_boss, my_trap, my_score);
           
            //check is game victory
            game_events.check_victory_condition(my_boss, my_score);
            //check is game over
            game_events.check_game_over_condition(my_tank, my_score);
        
        }
        break;

        //button "Mode" released
        case AC_DISPLAY_BUTON_MODE_RELEASED: { 
            //if is game over or victory
            if (game_events.isGameOver || game_events.isVictory) 
            {
                reset_game_bridge();
                timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);
                SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
            }
            //if tank live so fire 
            else {
                if(!my_tank.isExploding){
                     my_tank.tank_fire_cannon();
                }
            }
        }
        break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            //  UP -> Tank move forward
            if (!game_events.isGameOver && !game_events.isPaused) my_tank.moveForward();
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            // DOWN -> Tank move backward
            if (!game_events.isGameOver && !game_events.isPaused) my_tank.moveBackward();
            break;

        default:
            break;
    }
}
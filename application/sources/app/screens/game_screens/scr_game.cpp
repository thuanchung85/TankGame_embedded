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

bool isPaused = false;//game play or pause.

static uint8_t game_over_delay_counter = 0; //delay for animation
bool isGameOver = false; // state game over

bool isVictory = false;// state game victory

static void reset_game();
static void update_top_scores(uint32_t new_score);


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
    if(!my_boss.is_active){
        my_enemy.draw();
    }
    // draw Boss 
    my_boss.draw(); 
    //draw trap
    my_trap.draw();
    //draw ground 
    my_ground.draw();
    //draw score
    my_score.draw();

    // check Game Over
    if (isGameOver) 
    {
        //clear screen
        view_render.fillRect(0,0,130,65,BLACK); 
        //bitmap game over
        view_render.drawBitmap(0, 0, bitmap_game_over, 124, 60, WHITE);
        // score 
         view_render.setTextSize(1);
         view_render.setCursor(90, 30);
         view_render.print(my_score.current_score);
    } 
    //check victory game
    else if (isVictory)
    {
        //clear
        view_render.fillRect(0,0,130,65,BLACK); 
        // bitmap victory
        view_render.drawBitmap(0, 0, bitmap_victory, 124, 60, WHITE);
        
        // score
         view_render.setTextSize(1);
         view_render.setCursor(90, 30);
         view_render.print(my_score.current_score);
    }
    // check game Pause
    else if (isPaused) 
    {
        //clear
        view_render.fillRect(0,0,130,65,BLACK); 
        //bitmap pause game
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
            isGameOver = false;
            isPaused = false;

            //set timer
            timer_set(AC_TASK_DISPLAY_ID, 
              AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 
              60, // update each 60ms
              TIMER_PERIODIC);

            break;

        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: 
        {
            //game stop update when pause, game over, victory
            if (isGameOver || isPaused || isVictory) 
            {
                return; 
            }

            my_ground.update();//update scroliing ground
            my_tank.update(); //update tank

            // SPAWN BOSS when score at 200
            if (my_score.current_score >= 200 && !my_boss.is_active && my_boss.hp == my_boss.max_hp && !my_boss.is_exploding) {
                my_boss.spawn();
            }

            //update boss
            my_boss.update();  
            if(!my_boss.is_active){
                my_enemy.update();//update enemy tank 
            }
            my_tree.update();//tree update
            my_trap.update();//update rocket trap
            my_house.update();// update scrolling house
            my_mountain.update();//update mountain
            my_score.update();//update score 
    
            // TANK auto gun update attack air
            if (my_enemy.enemy_type == 1 && my_enemy.x < 120) { 
                my_tank.tank_fire_gun(5); 
            }

            // XỬ LÝ VA CHẠM TẬP TRUNG (COLLISION PHASE)!
            CollisionManager::check_all_collisions(my_tank, my_enemy, my_boss, my_trap, my_score);
           
             // check victory when boss die
            if (my_boss.isDie && !isVictory) {
                isVictory = true; // active VICTORY
                // --- SAVE score ---
                update_top_scores(my_score.current_score); 
                APP_DBG("GAME VICTORY! Boss is Dead.\n");
            }

            // check game over
            if (my_tank.isExploding) 
            {
                // delay 60ms for tank animation
                game_over_delay_counter++;
              
                if (game_over_delay_counter == 1) {
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                }

                //active game over , save score
                if (game_over_delay_counter >= 30) {
                    isGameOver = true;
                    update_top_scores(my_score.current_score);
                }
               
            }
           
          
        }
        break;

        //button "Mode" released
        case AC_DISPLAY_BUTON_MODE_RELEASED: { 
            //if game over or victory
            if (isGameOver || isVictory) {
                reset_game();
                timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);

                SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
                APP_DBG("TANK: Mode Button Released -> Returning to BANNER\n");
            
            }
            else //if game pause
            {
                isPaused = !isPaused;//toggle pause
                if (isPaused) 
                {
                    APP_DBG("GAME PAUSED\n");
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK); 
                } 
                else {
                    APP_DBG("GAME RESUMED\n");
                }
            }
        }
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            //  UP -> Tank move forward
            if (!isGameOver && !isPaused) my_tank.moveForward();
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            // DOWN -> Tank move backward
            if (!isGameOver && !isPaused) my_tank.moveBackward();
            break;

        default:
            break;
    }
}

static void reset_game(){
    game_over_delay_counter = 0;
    isGameOver = false;
    isPaused = false;
    isVictory = false;
    my_tank.reset();
    my_score.reset();
    my_house.reset();
    my_tree.reset();
    my_enemy.reset();
    my_trap.reset();
    my_mountain.reset();
    my_boss.reset(); 
    
}

static void update_top_scores(uint32_t new_score) {
    uint32_t top[3];
    eeprom_read(0x00, (uint8_t*)&top[0], 4);
    eeprom_read(0x04, (uint8_t*)&top[1], 4);
    eeprom_read(0x08, (uint8_t*)&top[2], 4);

    if (new_score > top[0]) {
        top[2] = top[1];
        top[1] = top[0];
        top[0] = new_score;
    } else if (new_score > top[1]) {
        top[2] = top[1];
        top[1] = new_score;
    } else if (new_score > top[2]) {
        top[2] = new_score;
    } else {
        return;
    }

    eeprom_write(0x00, (uint8_t*)&top[0], 4);
    eeprom_write(0x04, (uint8_t*)&top[1], 4);
    eeprom_write(0x08, (uint8_t*)&top[2], 4);
    APP_DBG("EEPROM: New High Score Saved!\n");
}
#include "scr_game_over.h"
//============  UPDATE TOP SCORE  =====//
void update_top_scores(uint32_t new_score) {
    uint32_t top[3];
    eeprom_read(0x00, (uint8_t*)&top[0], 4);
    eeprom_read(0x04, (uint8_t*)&top[1], 4);
    eeprom_read(0x08, (uint8_t*)&top[2], 4);

    if (new_score > top[0]) {
        top[2] = top[1]; top[1] = top[0]; top[0] = new_score;
    } else if (new_score > top[1]) {
        top[2] = top[1]; top[1] = new_score;
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

// ================ (DRAW)========================
static void view_scr_game_over()
{
    view_render.fillRect(0, 0, 130, 65, BLACK);
    view_render.drawBitmap(0, 0, bitmap_game_over, 124, 60, WHITE);
    view_render.setTextSize(1);
    view_render.setCursor(90, 30);
    view_render.print(static_score.current_score);
};

view_dynamic_t dyn_view_game_over = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game_over 
};


view_screen_t scr_game_over = {
    &dyn_view_game_over,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// ================logic (Event Handler)========================
void scr_game_over_handle(ak_msg_t* msg){
    switch (msg -> sig){
        //when this screen active
        case SCREEN_ENTRY:
            APP_DBG(">> Entered GAME OVER Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_GOODBYE);
            //update top score
            update_top_scores(static_score.current_score);
            // CLEAR ALL TIMER UPDATE 
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);
            break;

       
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE:
            break;

        //when button "MODE" released goto banner menu game for start again
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            break;
        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            break;

        default:
            break;
    }
}


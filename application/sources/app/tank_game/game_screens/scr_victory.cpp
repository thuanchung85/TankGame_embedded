#include "scr_victory.h"

// ================ (DRAW)========================
static void view_scr_victory()
{
    view_render.fillRect(0, 0, 130, 65, BLACK);
    view_render.drawBitmap(0, 0, bitmap_victory, 124, 60, WHITE);
    view_render.setTextSize(1);
    view_render.setCursor(90, 30);
    view_render.print(static_score.current_score);
};

view_dynamic_t dyn_view_victory = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_victory 
};


view_screen_t scr_victory = {
    &dyn_view_victory,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// ================logic (Event Handler)========================
void scr_victory_handle(ak_msg_t* msg){
    switch (msg -> sig){
        //when this screen active
        case SCREEN_ENTRY:
            APP_DBG(">> Entered GAME OVER Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_MERRY_CHRISTMAS);
            //update top score
            update_top_scores(static_score.current_score);
            // CLEAR ALL TIMER UPDATE 
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);
            break;

       
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE:
            break;

        //when button "MODE" released goto banner menu game for start again
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            task_post_pure_msg(TG_SCORE_TASK_ID, SCORE_RESET_SIG);
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


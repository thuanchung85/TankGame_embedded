#include "scr_topscore_game.h"


static uint32_t top_list[3] = {0, 0, 0}; 

static void view_scr_topscore_game()
{
    view_render.fillRect(0, 0, 130, 65, BLACK); 
    view_render.setTextSize(2);
    view_render.setCursor(10, 2);
    view_render.print("TOP SCORE");
    view_render.drawFastHLine(0, 20, 128, WHITE);
    view_render.setTextSize(1);

    //  1st
    view_render.setCursor(10, 25);
    view_render.print("1ST: ");
    view_render.setCursor(50, 25);
    view_render.print(top_list[0]); 

    //  2nd
    view_render.setCursor(10, 38);
    view_render.print("2ND: ");
    view_render.setCursor(50, 38);
    view_render.print(top_list[1]);

    //  3rd
    view_render.setCursor(10, 51);
    view_render.print("3RD: ");
    view_render.setCursor(50, 51);
    view_render.print(top_list[2]);

};


view_dynamic_t dyn_view_topscore_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_topscore_game 
};

view_screen_t scr_topscore_game = {
    &dyn_view_topscore_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


void scr_topscore_game_handle(ak_msg_t* msg){
    switch (msg -> sig){
        case SCREEN_ENTRY:
            APP_DBG(">> Entered TOP SCORE GAME Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
            eeprom_read(0x00, (uint8_t*)&top_list[0], 4);
            eeprom_read(0x04, (uint8_t*)&top_list[1], 4);
            eeprom_read(0x08, (uint8_t*)&top_list[2], 4);
            break;

        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        default:
            break;
    }
}


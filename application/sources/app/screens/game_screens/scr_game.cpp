#include "scr_game.h"
#include "game_objects/Ground.h"



static void view_scr_game()
{    
    

  
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
            break;

      

        case AC_DISPLAY_BUTON_MODE_DOWN:
            break;
        case AC_DISPLAY_BUTTON_MODE_HOLD:
             break;

        //button "Mode" released
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
          
        break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
             break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
             break;

        default:
            break;
    }
}
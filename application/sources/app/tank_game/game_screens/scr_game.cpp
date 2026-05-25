#include "scr_game.h"


static void view_scr_game()
{    
    
    ground_draw(); // Gọi mặt đất tự vẽ chính nó lên khung tranh chung
    tree_draw();   // Cây vẽ đè lên trên mặt đất 
    building_draw();   // Cây vẽ đè lên trên mặt đất 

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
        {
            APP_DBG(">> Entered TANK Screen Success!\n");
            // Khi vừa bước vào màn hình, phát lệnh khởi tạo mặt đất
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_SETUP_SIG);
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_SETUP_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_SETUP_SIG); 

            // kích hoạt hẹn giờ định kỳ trong Active Kernel cho game loop
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60, TIMER_PERIODIC);
        }
        break;

        // Tín hiệu update động cơ game loop
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: 
        {
            //APP_DBG(">>  TANK UPDATE!\n");
            // Cứ mỗi chu kỳ quét của màn hình, gửi 1 bức thư thúc giục Mặt đất dịch chuyển X
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_UPDATE_SIG);
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_UPDATE_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_UPDATE_SIG); 

        }
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
#include "scr_banner_game.h"

static uint8_t menu_index = 0; // 0: Start, 1: Top Score, 2: Exit

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_banner_game()
{
    
    view_render.fillRect(0,0,130,65,BLACK); // Xóa sạch buffer
    // Tọa độ (0,0), kích thước 124x60 theo bitmap bạn cung cấp
    view_render.drawBitmap(0, 0, bitmap_banner_game, 124, 60, WHITE);


    // 2. Vẽ các mục Menu
    view_render.setTextSize(1);
    
    // Mục 1: START
    view_render.setCursor(90, 30);
    view_render.print("START");
    
    // Mục 2: TOP SCORE
    view_render.setCursor(90, 42);
    view_render.print("TOP");
    
    // Mục 3: EXIT
    view_render.setCursor(90, 54);
    view_render.print("EXIT");

    // 3. Vẽ con trỏ (Cursor) dựa vào menu_index
    // Vẽ một hình tam giác nhỏ hoặc dấu '>' phía trước mục đang chọn
    int8_t cursor_y = 30 + (menu_index * 12); 
    view_render.setCursor(80, cursor_y);
    view_render.print(">");
    
};


// 2. ================Định nghĩa cấu trúc Màn hình (Data Structures)========================
//Định nghĩa đây là một đối tượng "động" (dynamic). 
//Nó liên kết trực tiếp với hàm vẽ view_scr_banner_game để hệ thống biết phải vẽ cái gì khi đối tượng này xuất hiện.
view_dynamic_t dyn_view_banner_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_banner_game // Trỏ đến hàm vẽ ở trên
};

//Đây là biến đại diện cho toàn bộ "màn hình" tank. 
//Nó chứa danh sách các item (ở đây chỉ có 1 item động, còn lại là ITEM_NULL). 
//Biến này chính là thứ bạn dùng để chuyển đổi giữa các màn hình trong game.
view_screen_t scr_banner_game = {
    &dyn_view_banner_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// 3. ================Bộ não xử lý logic (Event Handler)========================
void scr_banner_game_handle(ak_msg_t* msg){
    //msg -> sig: Đây là "tín hiệu" (signal) gửi đến từ hệ thống (Active Kernel).
    switch (msg -> sig){
        // Chạy khi bắt đầu vào màn hình này (ví dụ: khởi tạo âm thanh, biến số)
        //SCREEN_ENTRY: Một tín hiệu đặc biệt của AK, nó tự động kích hoạt khi màn hình banner vừa được bật lên.
        case SCREEN_ENTRY:
           menu_index = 0; // Reset về Start khi vào màn hình
            APP_DBG(">> Entered BANNER GAME Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
            break;

       

        // Khi nút "MODE" được thả ra
        //AC_DISPLAY_BUTON_MODE_RELEASED: Đây là sự kiện bấm nút. 
        //Khi bạn bấm và thả nút "Mode" trên mạch, code sẽ thực hiện lệnh bên trong.
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            // Xử lý khi chọn menu
            if (menu_index == 0) {
                // START GAME: Chuyển sang màn hình chơi game (scr_game)
                SCREEN_TRAN(scr_game_handle, &scr_game); 
            }
            else if (menu_index == 1) {
                // Xem TOP SCORE 
                SCREEN_TRAN(scr_topscore_game_handle, &scr_topscore_game);
            }
            else if (menu_index == 2) {
                // EXIT: Quay về màn hình Idle hoặc tắt game
                SCREEN_TRAN(scr_idle_handle, &scr_idle);
            }
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            if (menu_index > 0) menu_index--;
            else menu_index = 2; // Vòng lặp lên cuối
            BUZZER_PlaySound(BUZZER_SOUND_CLICK); // Tiếng tit nhỏ khi di chuyển
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            if (menu_index < 2) menu_index++;
            else menu_index = 0; // Vòng lặp về đầu
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        default:
            break;
    }
}


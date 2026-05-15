#include "scr_topscore_game.h"


static uint32_t top_list[3] = {0, 0, 0}; // Mảng tạm để hiển thị lên màn hình

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_topscore_game()
{
    view_render.fillRect(0, 0, 130, 65, BLACK); // Xóa sạch buffer

    // 1. Vẽ tiêu đề TOP SCORE (Chữ to)
    view_render.setTextSize(2);
    view_render.setCursor(10, 2);
    view_render.print("TOP SCORE");

    // Vẽ một đường gạch chân cho đẹp
    view_render.drawFastHLine(0, 20, 128, WHITE);

    // 2. Vẽ danh sách Nhất, Nhì, Ba
    view_render.setTextSize(1);

    // Hạng 1
    view_render.setCursor(10, 25);
    view_render.print("1ST: ");
    view_render.setCursor(50, 25);
    view_render.print(top_list[0]); // Sau này thay bằng biến lưu điểm thật

    // Hạng 2
    view_render.setCursor(10, 38);
    view_render.print("2ND: ");
    view_render.setCursor(50, 38);
    view_render.print(top_list[1]);

    // Hạng 3
    view_render.setCursor(10, 51);
    view_render.print("3RD: ");
    view_render.setCursor(50, 51);
    view_render.print(top_list[2]);

};


// 2. ================Định nghĩa cấu trúc Màn hình (Data Structures)========================
//Định nghĩa đây là một đối tượng "động" (dynamic). 
//Nó liên kết trực tiếp với hàm vẽ view_scr_banner_game để hệ thống biết phải vẽ cái gì khi đối tượng này xuất hiện.
view_dynamic_t dyn_view_topscore_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_topscore_game // Trỏ đến hàm vẽ ở trên
};

//Đây là biến đại diện cho toàn bộ "màn hình" tank. 
//Nó chứa danh sách các item (ở đây chỉ có 1 item động, còn lại là ITEM_NULL). 
//Biến này chính là thứ bạn dùng để chuyển đổi giữa các màn hình trong game.
view_screen_t scr_topscore_game = {
    &dyn_view_topscore_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// 3. ================Bộ não xử lý logic (Event Handler)========================
void scr_topscore_game_handle(ak_msg_t* msg){
    //msg -> sig: Đây là "tín hiệu" (signal) gửi đến từ hệ thống (Active Kernel).
    switch (msg -> sig){
        // Chạy khi bắt đầu vào màn hình này (ví dụ: khởi tạo âm thanh, biến số)
        //SCREEN_ENTRY: Một tín hiệu đặc biệt của AK, nó tự động kích hoạt khi màn hình banner vừa được bật lên.
        case SCREEN_ENTRY:
            APP_DBG(">> Entered TOP SCORE GAME Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);

            // Đọc 3 vị trí điểm (mỗi vị trí cách nhau 4 bytes)
            eeprom_read(0x00, (uint8_t*)&top_list[0], 4);
            eeprom_read(0x04, (uint8_t*)&top_list[1], 4);
            eeprom_read(0x08, (uint8_t*)&top_list[2], 4);
            
            break;

       

        // Khi nút "MODE" được thả ra
        //AC_DISPLAY_BUTON_MODE_RELEASED: Đây là sự kiện bấm nút. 
        //Khi bạn bấm và thả nút "Mode" trên mạch, code sẽ thực hiện lệnh bên trong.
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            BUZZER_PlaySound(BUZZER_SOUND_CLICK); // Tiếng tit nhỏ khi di chuyển
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        default:
            break;
    }
}


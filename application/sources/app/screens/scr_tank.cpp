#include "scr_tank.h"

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_tank(){
    //Lệnh này vẽ hình ảnh tank lên màn hình. 
    // 0, 0: Tọa độ bắt đầu (góc trên cùng bên trái).
    // tank: Dữ liệu hình ảnh lấy từ screens_bitmap.h mà bạn thấy trong file header.
    // 30, 30: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 128x64). tank size 30x30
    // WHITE: Màu sắc của hình ảnh.
    view_render.drawBitmap(20,30,bitmap_tank,30,30,WHITE);
};


// 2. ================Định nghĩa cấu trúc Màn hình (Data Structures)========================
//Định nghĩa đây là một đối tượng "động" (dynamic). 
//Nó liên kết trực tiếp với hàm vẽ view_scr_tank để hệ thống biết phải vẽ cái gì khi đối tượng này xuất hiện.
view_dynamic_t dyn_view_tank = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_tank // Trỏ đến hàm vẽ ở trên
};

//Đây là biến đại diện cho toàn bộ "màn hình" tank. 
//Nó chứa danh sách các item (ở đây chỉ có 1 item động, còn lại là ITEM_NULL). 
//Biến này chính là thứ bạn dùng để chuyển đổi giữa các màn hình trong game.
view_screen_t scr_tank = {
    &dyn_view_tank,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// 3. ================Bộ não xử lý logic (Event Handler)========================
void scr_tank_handle(ak_msg_t* msg){
    //msg -> sig: Đây là "tín hiệu" (signal) gửi đến từ hệ thống (Active Kernel).
    switch (msg -> sig){
        // Chạy khi bắt đầu vào màn hình này (ví dụ: khởi tạo âm thanh, biến số)
        //SCREEN_ENTRY: Một tín hiệu đặc biệt của AK, nó tự động kích hoạt khi màn hình tank vừa được bật lên.
        case SCREEN_ENTRY:
            APP_DBG(">> Entered TANK Screen Success!\n");
            APP_DBG(">> Screen Bitmap Size: bitmap_TANK = 120 bytes\n");
            break;

        // Khi nút "MODE" được thả ra
        //AC_DISPLAY_BUTON_MODE_RELEASED: Đây là sự kiện bấm nút. 
        //Khi bạn bấm và thả nút "Mode" trên mạch, code sẽ thực hiện lệnh bên trong.
        case AC_DISPLAY_BUTON_MODE_RELEASED: { 
            //SCREEN_TRAN(scr_idle_handle, &scr_idle): Đây là lệnh Chuyển cảnh (Transition).
            // Nó sẽ thoát khỏi màn hình tank và quay về màn hình chờ (scr_idle).
            SCREEN_TRAN(scr_idle_handle, &scr_idle);
            APP_DBG("TANK: Mode Button Released -> Returning to Idle\n");
            }
            break;

        default:
            break;
    }
}
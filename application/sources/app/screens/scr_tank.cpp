#include "scr_tank.h"
#include "game_objects/Tank.h"
#include "game_objects/Ground.h"
#include "game_objects/Tree.h"
#include "game_objects/Mountain.h"
#include "game_objects/House.h"


//================================================GAME LOOP AREA=======================================================//
//============KHAI BÁO: Biến và object ========//
static Tank my_tank;//xe tank
static Ground my_ground;//mặt đất cuộn
static Tree my_tree;//cây trên mặt đất
static Mountain my_mountain;//núi ở xa
static House my_house;//nhà ở xa

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_tank(){
    //chú ý phải vẽ theo thứ tự object xa nhất ưu tiên vẽ trước, object gần nhất vẽ sau cùng.
    //draw mountain
    my_mountain.draw();

    //draw house
    my_house.draw();

    //draw tree
    my_tree.draw();

    //draw xe tank
    my_tank.draw();

    //draw ground đang cuộn
    my_ground.draw();

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
           
            // Bắt đầu gửi tin nhắn cập nhật màn hình định kỳ
            timer_set(AC_TASK_DISPLAY_ID, 
              AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, // Bạn có thể định nghĩa signal mới này
              30, // Cập nhật mỗi 30ms
              TIMER_PERIODIC);

            break;

        //TANK UPDATE LOOP
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: {
            APP_DBG("TANK: I alive now tick!\n");

            my_ground.update();//update thông số cuộn mặt đất, để nó chạy từ phải qua trái
            my_tank.update(); // Cập nhật các hiệu ứng của tank nếu có
            my_tree.update();//di chuyển cây 

            my_house.update();//nhà update vị trí
            my_mountain.update();//núi ở xa nhất update chậm nhất
        }
        break;

        // Khi nút "MODE" được thả ra
        //AC_DISPLAY_BUTON_MODE_RELEASED: Đây là sự kiện bấm nút. 
        //Khi bạn bấm và thả nút "Mode" trên mạch, code sẽ thực hiện lệnh bên trong.
        case AC_DISPLAY_BUTON_MODE_RELEASED: { 
            // Phải gỡ bỏ timer trước khi chuyển màn hình
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);

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
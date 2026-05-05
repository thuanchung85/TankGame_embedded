#include "scr_tank.h"
//============================================CLASSES AREA=======================================//
//--------------TANK CLASS--------------------------//
class Tank //class xe tank
{
public:
    int x;//game play chỉ có 2 nut bấm cho xe tank tới lui né tránh đạn
    int speed;
    
    // Khởi tạo (Constructor)
    Tank() {
        x = 20;        // Vị trí X mặc định   
        speed = 0;
    }

    // Hàm cập nhật trạng thái (giống hàm moving của ball)
    void update() {
        // Bạn có thể thêm logic rung lắc khi chạy ở đây
        // Hoặc logic nhảy (jump) nếu muốn
    }

    // Hàm vẽ tank
    void draw() {
        //Lệnh này vẽ hình ảnh tank lên màn hình. 
        // 20, 30: Tọa độ bắt đầu (góc trên cùng bên trái).
        // bitmap_tank: Dữ liệu hình ảnh lấy từ screens_bitmap.h mà bạn thấy trong file header.
        // 30, 30: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60). tank size 30x30
        // WHITE: Màu sắc của hình ảnh.
        view_render.drawBitmap(x, 30, bitmap_tank, 30, 30, WHITE);
    }
};
//--------------SCROLLING GROUND CLASS--------------------------//
class ScrollingGround //class mặt đất cuộn chạy liên tục qua tay trái
{
    public:
    int scroll_offset;
    
    // Khởi tạo (Constructor)
    ScrollingGround() { 
        scroll_offset = 0;
    }

    // Hàm cập nhật trạng thái (giống hàm moving của ball)
    void update() {
        //scroll mặt đất
        scroll_offset++;
        if (scroll_offset >= 24) { // 24 là độ rộng mảng ground của bạn
            scroll_offset = 0;
        }
    }

    // Hàm vẽ 
    void draw() {
        // Vẽ mặt đất bằng các khối 24x8 pixel
        // Tọa độ y = 52 (nằm dưới chân xe tank)
        for (int x = 0; x < 124; x += 24) { 
            // Lưu ý: chiều rộng là 24, chiều cao là 8
            view_render.drawBitmap(x - (scroll_offset % 24), 52, bitmap_ground, 24, 8, WHITE);
        }
    }
};

//================================================GAME LOOP AREA=======================================================//
//============KHAI BÁO: Biến và object ========//
static Tank my_tank;//xe tank
static ScrollingGround my_ground;
//static int scroll_offset = 0;//biến scroll mặt đất


// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_tank(){
    
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
            APP_DBG(">> Screen Bitmap Size: bitmap_TANK = 120 bytes\n");
            APP_DBG(">> Screen Bitmap Size: bitmap_ground = 24 bytes\n");

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
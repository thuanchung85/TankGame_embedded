#include "scr_tank.h"
//============================================CLASSES AREA=======================================//
//--------------TANK CLASS--------------------------//
class Tank //class xe tank
{
public:
    int8_t x;//game play chỉ có 2 nut bấm cho xe tank tới lui né tránh đạn
    int8_t speed;
    
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
    int8_t scroll_offset;
    
    // Khởi tạo (Constructor)
    ScrollingGround() { 
        scroll_offset = 0;
    }

    // Hàm cập nhật trạng thái 
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
        for (short x = 0; x < 124; x += 24) { 
            // Lưu ý: chiều rộng là 24, chiều cao là 8
            view_render.drawBitmap(x - (scroll_offset % 24), 52, bitmap_ground, 24, 8, WHITE);
        }
    }
};

//--------------THE TREE------------------------------//
class Tree //class cây 
{
public:
    short x;//vị trí mọc cây dừa trên đất
    int8_t tree_type;

    // Khởi tạo (Constructor)
    Tree() {
        x = 130; 
        tree_type = 0;       
    }

    // Hàm cập nhật trạng thái 
    void update() {
        //scroll cây dừa trên mặt đất, cho nó di chuyển từ phải sang trái
        x--;
        // Nếu cây dừa đã trôi hết qua trái màn hình
        if (x < -35) {
            x = 124 + 35; // Reset về bên phải để xuất hiện lại

            tree_type = rand() % 3;
        }
    }

    // Hàm vẽ cây dừa
    void draw() {

        const unsigned char* tree_bitmaps[] = {
            bitmap_coconutTree, 
            bitmap_dryTree, 
            bitmap_grassesTree
        };

        // x, 52 - 31: Tọa độ bắt đầu.
        // bitmap_coconuttree: Dữ liệu hình ảnh lấy từ screens_bitmap.h  
        // 30, 31: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60).
        // WHITE: Màu sắc của hình ảnh.
        view_render.drawBitmap(x, 52 - 31, tree_bitmaps[tree_type], 30, 31, WHITE);
    }
};

//--------------THE MOUNTAIN ------------------------------//
class TheMountain //class núi
{
public:
    short x;//vị trí 
    int8_t frame_count;
    
    
    // Khởi tạo (Constructor)
    TheMountain() {
        x = 200;        
    }

    // Hàm cập nhật trạng thái 
    void update() {
        //scroll núi trên mặt đất, cho nó di chuyển từ phải sang trái
        //núi ở xa chạy chậm hơn mặt đất và cây dừa 3 frame.
        frame_count++;
        if(frame_count >= 6)
        {
            x--;
            if (x < -120) 
            {
                x = 200 + 90; // Reset về bên phải để xuất hiện lại
            }
            frame_count = 0;
        }
             
      
    }

    // Hàm vẽ núi
    void draw() {
        // x, 0: Tọa độ bắt đầu.
        // bitmap_theMountain: Dữ liệu hình ảnh lấy từ screens_bitmap.h
        // 90, 42: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60).
        // WHITE: Màu sắc của hình ảnh.
        view_render.drawBitmap(x, 0, bitmap_theMountain, 90, 42, WHITE);
    }
};

//--------------THE HOUSE ------------------------------//
class House //class căn nhà 
{
public:
    short x;//vị trí 
    int8_t frame_count;
    int8_t house_type; // Biến để lưu kiểu nhà hiện tại (0 đến 3)

    // Khởi tạo (Constructor)
    House() {
        x = 180; 
        house_type = 0; // Mặc định là nhà số 1       
    }

    // Hàm cập nhật trạng thái 
    void update() {
        
        frame_count++;
        if(frame_count >= 2)
        {
            x--;
            if (x < -120) 
            {
                x = 130 + (rand() % 100); // Reset về bên phải để xuất hiện lại

                // --- PHẦN RANDOM QUAN TRỌNG ---
                // rand() % 4 sẽ trả về một số ngẫu nhiên từ 0, 1, 2, 3
                house_type = rand() % 4;
            }
            frame_count = 0;
        }
    }

    // Hàm vẽ căn nhà 
    void draw() {
        // Mảng chứa các bitmap nhà của bạn
        const unsigned char* house_bitmaps[] = {
            bitmap_house1, 
            bitmap_house2, 
            bitmap_house3, 
            bitmap_house4
        };
        view_render.drawBitmap(x, 10, house_bitmaps[house_type], 30, 30, WHITE);
    }
};


//================================================GAME LOOP AREA=======================================================//
//============KHAI BÁO: Biến và object ========//
static Tank my_tank;//xe tank
static ScrollingGround my_ground;//mặt đất cuộn
static Tree my_coconutTree;//cây dừa
static TheMountain my_mountain;//núi ở xa
static House my_house;//nhà ở xa

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_tank(){
    //chú ý phải vẽ theo thứ tự object xa nhất ưu tiên vẽ trước, object gần nhất vẽ sau cùng.
    //draw mountain
    my_mountain.draw();

    //draw house
    my_house.draw();

    //draw coconut tree
    my_coconutTree.draw();

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
            my_coconutTree.update();//di chuyển cây dừa

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
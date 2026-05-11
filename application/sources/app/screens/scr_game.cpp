#include "scr_game.h"
#include "game_objects/Tank.h"
#include "game_objects/Ground.h"
#include "game_objects/Tree.h"
#include "game_objects/Mountain.h"
#include "game_objects/House.h"
#include "game_objects/Score.h"
#include "game_objects/Enemy.h"
#include "game_objects/Trap.h"

//================================================GAME LOOP AREA=======================================================//
//============KHAI BÁO: Biến và object ========//
static Tank my_tank;//xe tank
static Ground my_ground;//mặt đất cuộn
static Tree my_tree;//cây trên mặt đất
static Mountain my_mountain;//núi ở xa
static House my_house;//nhà ở xa
static Score my_score;//điểm số
static Enemy my_enemy; //địch
static Trap my_trap; //các trap như là rocket

// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_game(){
    //chú ý phải vẽ theo thứ tự object xa nhất ưu tiên vẽ trước, object gần nhất vẽ sau cùng.
    //draw mountain
    my_mountain.draw();

    //draw house
    my_house.draw();

    //draw tree
    my_tree.draw();

    //draw xe tank
    my_tank.draw();

    //draw địch
    my_enemy.draw();

     //draw trap
    my_trap.draw();

    //draw ground đang cuộn
    my_ground.draw();

    //draw score
    my_score.draw();

};


// 2. ================Định nghĩa cấu trúc Màn hình (Data Structures)========================
//Định nghĩa đây là một đối tượng "động" (dynamic). 
//Nó liên kết trực tiếp với hàm vẽ view_scr_tank để hệ thống biết phải vẽ cái gì khi đối tượng này xuất hiện.
view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game // Trỏ đến hàm vẽ ở trên
};

//Đây là biến đại diện cho toàn bộ "màn hình" tank. 
//Nó chứa danh sách các item (ở đây chỉ có 1 item động, còn lại là ITEM_NULL). 
//Biến này chính là thứ bạn dùng để chuyển đổi giữa các màn hình trong game.
view_screen_t scr_game = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// 3. ================Bộ não xử lý logic (Event Handler)========================
void scr_game_handle(ak_msg_t* msg){
    //msg -> sig: Đây là "tín hiệu" (signal) gửi đến từ hệ thống (Active Kernel).
    switch (msg -> sig){
        // Chạy khi bắt đầu vào màn hình này (ví dụ: khởi tạo âm thanh, biến số)
        //SCREEN_ENTRY: Một tín hiệu đặc biệt của AK, nó tự động kích hoạt khi màn hình tank vừa được bật lên.
        case SCREEN_ENTRY:
            APP_DBG(">> Entered TANK Screen Success!\n");
           
            // Bắt đầu gửi tin nhắn cập nhật màn hình định kỳ
            timer_set(AC_TASK_DISPLAY_ID, 
              AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, // Bạn có thể định nghĩa signal mới này
              60, // Cập nhật mỗi 60ms, chú ý update thời gian càng ngắn thì bộ RAM càng nhanh bị tràn
              TIMER_PERIODIC);

            break;

        //TANK UPDATE LOOP
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: {
            //APP_DBG("TANK: I alive now tick!\n");

           

            my_ground.update();//update thông số cuộn mặt đất, để nó chạy từ phải qua trái
            my_tank.update(); // Cập nhật các hiệu ứng của tank nếu có
            // Tự động bắn Gun nếu thấy máy bay
            if (my_enemy.enemy_type == 1 && my_enemy.x < 120) { 
                // Truyền y của máy bay vào (y=5 trong class Enemy của bạn)
                my_tank.tank_fire_gun(5); 
            }
            // Kiểm tra va chạm giữa đạn Canon và Enemy tank, mine, troop
            if (my_tank.my_canon_bullets.is_active) 
            {
                 if(my_enemy.enemy_type != 1)
                {
                    // Giả sử viên đạn canon của bạn có kích thước 5x3 như trong code vẽ
                    if (my_enemy.checkCollision(my_tank.my_canon_bullets.x, 
                                                my_tank.my_canon_bullets.y, 5, 3)) {
                        // 1. Vô hiệu hóa viên đạn ngay lập tức
                        my_tank.my_canon_bullets.is_active = false;
                        
                        // 2. Trừ máu kẻ địch
                        my_enemy.hp--;

                        // 3. Kiểm tra xem kẻ địch đã chết chưa
                        if (my_enemy.hp <= 0) {
                            // Kẻ địch bị tiêu diệt
                            my_enemy.isExploding = true;
                            my_enemy.explosionTimer = 0;
                            my_score.add(); 
                            BUZZER_PlayTones(tones_3beep); // Tiếng bíp báo hiệu tiêu diệt
                        } else {
                            // Kẻ địch còn máu (xe tank trúng phát đầu)
                            
                        }
                    }
                }
            }
            //kiểm tra va chạm gun và máy bay (1)
            if(my_tank.my_gun_bullets.is_active)
            {
                if(my_enemy.enemy_type == 1)
                {
                    // Giả sử viên đạn gun của bạn có kích thước 2x1 như trong code vẽ
                    if (my_enemy.checkCollision(my_tank.my_gun_bullets.x, 
                                                my_tank.my_gun_bullets.y, 2, 1)) {
                        // 1. Vô hiệu hóa viên đạn ngay lập tức
                        my_tank.my_gun_bullets.is_active = false;
                        
                        // 2. Trừ máu kẻ địch
                        my_enemy.hp--;

                        // 3. Kiểm tra xem kẻ địch đã chết chưa
                        if (my_enemy.hp <= 0) {
                            // Kẻ địch bị tiêu diệt
                            my_enemy.isExploding = true;
                            my_enemy.explosionTimer = 0;
                            my_score.add(); 
                            BUZZER_PlayTones(tones_3beep); // Tiếng bíp báo hiệu tiêu diệt
                        } else {
                            // Kẻ địch còn máu (xe tank trúng phát đầu)
                            
                        }
                    }
                }
            }

            my_enemy.update();//update xe tank địch

            my_tree.update();//di chuyển cây 
           
            my_trap.update();
            my_house.update();//nhà update vị trí
            my_mountain.update();//núi ở xa nhất update chậm nhất

            my_score.update();//update score
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

        case AC_DISPLAY_BUTON_UP_RELEASED:
            // Khi bấm nút UP -> Tank tiến lên
            my_tank.moveForward();
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            // Khi bấm nút DOWN -> Tank lùi lại
            my_tank.moveBackward();
            break;

        default:
            break;
    }
}
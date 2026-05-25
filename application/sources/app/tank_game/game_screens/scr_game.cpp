#include "scr_game.h"

// Hàm kiểm tra 2 hình chữ nhật có đè lên nhau không
static bool check_collision(int16_t x1, int16_t y1, int16_t w1, int16_t h1,
                            int16_t x2, int16_t y2, int16_t w2, int16_t h2) 
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

//  ================draw game objects ================
static void view_scr_game()
{      
    mountain_draw(); 
    building_draw(); 
    ground_draw();   
    tank_draw();     
    enemy_draw();   // <<< Vẽ Kẻ địch (Vẽ trước cây để cây che nếu muốn, hoặc vẽ sau cùng)
    tree_draw();     
    cannon_bullet_draw(); // Vẽ đạn

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
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_SETUP_SIG); 
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_SETUP_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_SETUP_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_SETUP_SIG); 
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_SETUP_SIG); 


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
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_UPDATE_SIG);
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_UPDATE_SIG);
            task_post_pure_msg(TG_ENEMY_TASK_ID, ENEMY_UPDATE_SIG); 

            task_post_pure_msg(TG_TREE_TASK_ID, TREE_UPDATE_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_UPDATE_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_UPDATE_SIG); 

            // Đưa extern các biến tĩnh vào để dùng chung dữ liệu
            extern tank_t static_tank;
            extern cannon_bullet_t static_cannon_bullet;
            extern enemy_t static_enemy;
            // ==========================================================
            // XỬ LÝ VA CHẠM 1: ĐẠN TA BẮN TRÚNG ĐỊCH
            // ==========================================================
            if (static_cannon_bullet.is_active && !static_enemy.isExploding) 
            {
                // Lấy kích thước hộp va chạm (Bounding Box) của địch tùy theo loại
                int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
                if (static_enemy.enemy_type == 1) enemy_y = 5;  // Máy bay trên cao
                else if (static_enemy.enemy_type == 3) { enemy_w = 15; enemy_y = 34; } // Lính

                // Check xem viên đạn (5x3) có chạm vào Địch không
                if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                                    static_enemy.x, enemy_y, enemy_w, enemy_h)) 
                {
                    // Tắt viên đạn đi ngay lập tức
                    static_cannon_bullet.is_active = false;
                    
                    // Giảm máu địch
                    if (static_enemy.hp > 0) static_enemy.hp--;

                    // Nếu hết máu thì cho địch nổ tung!
                    if (static_enemy.hp == 0) {
                        static_enemy.isExploding = true;
                        static_enemy.explosionTimer = 0;
                        //APP_DBG(">> ENEMY DESTROYED!\n");
                    } else {
                        //APP_DBG(">> ENEMY HIT! HP REMAINING: %d\n", static_enemy.hp);
                    }
                }
            }

            // ==========================================================
            // XỬ LÝ VA CHẠM 2: ĐỊCH ĐÂM TRÚNG XE TĂNG TA
            // ==========================================================
            if (!static_tank.isExploding && !static_enemy.isExploding) 
            {
                int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
                if (static_enemy.enemy_type == 1) enemy_y = 5; 
                else if (static_enemy.enemy_type == 3) { enemy_w = 15; enemy_y = 34; }

                // Check xem Địch có đâm trúng Xe tăng ta (30x30 tại Y=30) không
                if (check_collision(static_tank.x, 30, 30, 30,
                                    static_enemy.x, enemy_y, enemy_w, enemy_h)) 
                {
                    // Xe tăng ta nổ tung, trò chơi kết thúc hoặc reset
                    static_tank.isExploding = true;
                    static_tank.explosion_timer = 0;
                    //APP_DBG(">> GAME OVER - TANK CRASHED!\n");
                    static_enemy.isExploding = true;
                    static_enemy.explosionTimer = 0;
                    //APP_DBG(">> ENEMY DESTROYED!\n");
                }
            }
        }
        break;

        case AC_DISPLAY_BUTON_MODE_DOWN:
            break;
        case AC_DISPLAY_BUTTON_MODE_HOLD:
             break;

        //button "Mode" released
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
          task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_FIRE_SIG);
        break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_FORWARD_SIG);
        break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_BACKWARD_SIG);
        break;

        default:
            break;
    }
}
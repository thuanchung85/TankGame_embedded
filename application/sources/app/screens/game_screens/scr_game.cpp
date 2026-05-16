#include "scr_game.h"
#include "game_objects/Tank.h"
#include "game_objects/Ground.h"
#include "game_objects/Tree.h"
#include "game_objects/Mountain.h"
#include "game_objects/House.h"
#include "game_objects/Score.h"
#include "game_objects/Enemy.h"
#include "game_objects/Trap.h"
#include "game_objects/Boss.h" // Nhớ include vào nhé Luong

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
static Boss my_boss; // Khai báo object Boss

bool isPaused = false;//biến trạng thái game play hay pause.

static uint8_t game_over_delay_counter = 0; // Biến đếm thời gian chờ Game Over
bool isGameOver = false; // Trạng thái kết thúc game

bool isVictory = false;// Trạng thái game victory

static void reset_game();
static void update_top_scores(uint32_t new_score);


// 1. ================Phần Hiển thị (Rendering)========================
//Đây là hàm vẽ. Mỗi khi màn hình cần làm mới, hệ thống sẽ gọi hàm này.
static void view_scr_game()
{
    
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
    if(!my_boss.is_active){
        my_enemy.draw();
    }

    // Vẽ Boss tại đây
    my_boss.draw(); 

    //draw trap
    my_trap.draw();
    

    //draw ground đang cuộn
    my_ground.draw();

    //draw score
    my_score.draw();

    // Kiểm tra trạng thái Game Over
    if (isGameOver) 
    {
        view_render.fillRect(0,0,130,65,BLACK); // Xóa sạch buffer
        // Vẽ bitmap Game Over lên trung tâm (0,0)
        view_render.drawBitmap(0, 0, bitmap_game_over, 124, 60, WHITE);
        
        // Bạn có thể in thêm điểm số cuối cùng ở dưới banner
         view_render.setTextSize(1);
         view_render.setCursor(90, 30);
         view_render.print(my_score.current_score);
    } 
    //nếu là victory game
    else if (isVictory){
        view_render.fillRect(0,0,130,65,BLACK); // Xóa sạch buffer
        // Vẽ bitmap Game Over lên trung tâm (0,0)
        view_render.drawBitmap(0, 0, bitmap_victory, 124, 60, WHITE);
        
        // Bạn có thể in thêm điểm số cuối cùng ở dưới banner
         view_render.setTextSize(1);
         view_render.setCursor(90, 30);
         view_render.print(my_score.current_score);
    }

    // Nếu đang Pause, vẽ bitmap ghi đè lên toàn bộ màn hình
    else if (isPaused) 
    {
        view_render.fillRect(0,0,130,65,BLACK); // Xóa sạch buffer
        // Tọa độ (0,0), kích thước 124x60 theo bitmap bạn cung cấp
        view_render.drawBitmap(0, 0, bitmap_game_pause, 124, 60, WHITE);
    }

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
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
            isGameOver = false;//game over false khi mới vào game
            isPaused = false;//resume game

            // Bắt đầu gửi tin nhắn cập nhật màn hình định kỳ
            timer_set(AC_TASK_DISPLAY_ID, 
              AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, // Bạn có thể định nghĩa signal mới này
              60, // Cập nhật mỗi 60ms, chú ý update thời gian càng ngắn thì bộ RAM càng nhanh bị tràn
              TIMER_PERIODIC);

            break;

        //TANK UPDATE LOOP
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: {
            //APP_DBG("TANK: I alive now tick!\n");
            // NẾU ĐANG PAUSE hoặc bị game over -> THOÁT KHÔNG XỬ LÝ LOGIC
            if (isGameOver || isPaused || isVictory) {
                return; 
            }
           

            my_ground.update();//update thông số cuộn mặt đất, để nó chạy từ phải qua trái
            my_tank.update(); // Cập nhật các hiệu ứng của tank nếu có


            // 1. KIỂM TRA ĐIỀU KIỆN XUẤT HIỆN BOSS
            if (my_score.current_score >= 20 && !my_boss.is_active && my_boss.hp == my_boss.max_hp && !my_boss.is_exploding) {
                my_boss.spawn();
                // Tùy chọn: Khi Boss xuất hiện, bạn có thể tạm thời cho ngưng xuất hiện Enemy thường
            }

            // 2. UPDATE BOSS LOGIC
            my_boss.update();
            
            // 3. KIỂM TRA VA CHẠM ĐẠN CANON TRÚNG BOSS
            if (my_tank.my_canon_bullets.is_active && my_boss.is_active) {
                if (my_boss.check_collision(my_tank.my_canon_bullets.x, my_tank.my_canon_bullets.y, 5, 3)) {
                    my_tank.my_canon_bullets.is_active = false; // Mất viên đạn
                    my_boss.lose_hp(1); // Boss mất 1 máu
                    BUZZER_PlaySound(BUZZER_SOUND_BANG); // Kêu bíp trúng đạn
                    
                    if (my_boss.hp <= 0) { // Nếu diệt được Boss
                        my_score.current_score += 100; // Thưởng hẳn 100 điểm!
                        //delay for animation
                        game_over_delay_counter++;
                        if (game_over_delay_counter == 1) {
                            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                        }
                        if (game_over_delay_counter >= 30) {
                            isVictory = true;
                            // --- LƯU ĐIỂM VÀO EEPROM TẠI ĐÂY ---
                            update_top_scores(my_score.current_score);
                        }

                    }
                }
            }
    
            // 4. KIỂM TRA VA CHẠM XE TANK MÌNH ĐÂM VÀO BOSS
            if (my_boss.is_active && !my_tank.isExploding) {
                // Coi như đâm vào Boss là dính sát thương cực nặng
                if (my_boss.check_collision(my_tank.x, 33, 25, 15)) { // Tọa độ Y xe tank bạn đang để check va chạm là 33
                    my_tank.lossHP();
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
                }
            }

            // KIỂM TRA ĐIỀU KIỆN THUA CUỘC
            if (my_tank.isExploding) {
                // Trong khi tank đang nổ, các object khác vẫn có thể update hoặc đứng yên tùy bạn
                // Chúng ta sẽ tăng biến đếm lên theo mỗi chu kỳ 60ms
                game_over_delay_counter++;
              
                if (game_over_delay_counter == 1) {
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                }

                if (game_over_delay_counter >= 30) {
                    isGameOver = true;
                    // --- LƯU ĐIỂM VÀO EEPROM TẠI ĐÂY ---
                    update_top_scores(my_score.current_score);
                }
               
            }

            // Lấy thông tin kích thước Enemy để check va chạm
            int8_t eW =25;
            int8_t eH = 21;
            int8_t eY = 33;
            switch (my_enemy.enemy_type) {
                case 0: eW = 25; eH = 21; eY = 33; break; // Tank địch
                case 1: eW = 25; eH = 21; eY = 5;  break; // Máy bay
                case 2: eW = 22; eH = 21; eY = 33; break; // Mìn
                case 3: eW = 15; eH = 21; eY = 34; break; // Lính
            }

            // Kiểm tra va chạm giữa Tank mình và Enemy
            // Lưu ý: Chỉ check khi enemy chưa nổ và tank chưa nổ
            if (!my_enemy.isExploding) 
            {
                if (my_tank.checkCollisionWithEnemy(my_enemy.x, eY, eW, eH) && my_enemy.hp > 0) 
                {
                    
                    // CẢ HAI CÙNG NỔ!
                    my_enemy.isExploding = true;
                    my_enemy.explosionTimer = 0;
                    
                    // Giả sử bạn thêm biến isExploding cho Tank
                    my_tank.lossHP();
                    //my_tank.isExploding = true; 
                    
                    // Phát âm thanh nổ lớn
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
                }
            }


            // Tự động bắn Gun nếu thấy máy bay
            if (my_enemy.enemy_type == 1 && my_enemy.x < 120) { 
                // Truyền y của máy bay vào (y=5 trong class Enemy của bạn)
                my_tank.tank_fire_gun(5); 
            }
            // Kiểm tra va chạm giữa đạn Canon và Enemy tank, mine, troop
            if (my_tank.my_canon_bullets.is_active) 
            {
                 if(my_enemy.enemy_type != 1 && my_enemy.hp > 0)
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
                            BUZZER_PlaySound(BUZZER_SOUND_BANG); // Tiếng bíp báo hiệu tiêu diệt
                        } else {
                            // Kẻ địch còn máu (xe tank trúng phát đầu)
                            
                        }
                    }
                }
            }
            //kiểm tra va chạm gun và máy bay (1)
            if(my_tank.my_gun_bullets.is_active)
            {
                if(my_enemy.enemy_type == 1 && my_enemy.hp > 0)
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
                            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); // Tiếng bíp báo hiệu tiêu diệt
                        } else {
                            // Kẻ địch còn máu (xe tank trúng phát đầu)
                            
                        }
                    }
                }
            }

            if(!my_boss.is_active){
                my_enemy.update();//update xe tank địch
            }
          

            my_tree.update();//di chuyển cây 
           
           
            my_trap.update();//di chuyển rocket
            
            // Kiểm tra va chạm với Rocket từ trên trời
            if (!my_tank.isExploding) {
                if (my_trap.checkCollisionWithTank(my_tank.x, 30, 25, 15)) {
                    // 1. Tank mất máu
                    my_tank.lossHP(); 
                    
                    // 2. Rocket nổ ngay lập tức (Reset về vị trí cũ)
                    my_trap.y = -20;
                    my_trap.x = rand() % 100;
                    
                    // 3. Hiệu ứng âm thanh và hình ảnh
                    BUZZER_PlaySound(BUZZER_SOUND_BANG);
                    //APP_DBG("TANK HIT BY ROCKET!\n");
                }
            }

            my_house.update();//nhà update vị trí
            my_mountain.update();//núi ở xa nhất update chậm nhất

            my_score.update();//update score
        }
        break;

        // Khi nút "MODE" được thả ra
        //AC_DISPLAY_BUTON_MODE_RELEASED: Đây là sự kiện bấm nút. 
        //Khi bạn bấm và thả nút "Mode" trên mạch, code sẽ thực hiện lệnh bên trong.
        case AC_DISPLAY_BUTON_MODE_RELEASED: { 
            
            if (isGameOver || isVictory) {
                // Nếu đang Game Over, bấm MODE để RESTART hoặc về IDLE
                reset_game();
                // Phải gỡ bỏ timer trước khi chuyển màn hình
                timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE);

                //SCREEN_TRAN(scr_idle_handle, &scr_idle): Đây là lệnh Chuyển cảnh (Transition).
                // Nó sẽ thoát khỏi màn hình tank và quay về màn hình chờ (scr_idle).
                SCREEN_TRAN(scr_banner_game_handle, &scr_banner_game);
                APP_DBG("TANK: Mode Button Released -> Returning to BANNER\n");
            
            }
            else 
            {
                isPaused = !isPaused; // Đảo trạng thái: Play <-> Pause
                if (isPaused) 
                {
                    APP_DBG("GAME PAUSED\n");
                    BUZZER_PlaySound(BUZZER_SOUND_CLICK); // Kêu bíp nhẹ báo hiệu
                } 
                else {
                    APP_DBG("GAME RESUMED\n");
                }
            }
        }
            break;

        case AC_DISPLAY_BUTON_UP_RELEASED:
            // Khi bấm nút UP -> Tank tiến lên
            if (!isGameOver && !isPaused) my_tank.moveForward();
            break;

        case AC_DISPLAY_BUTON_DOWN_RELEASED:
            // Khi bấm nút DOWN -> Tank lùi lại
            if (!isGameOver && !isPaused) my_tank.moveBackward();
            break;

        default:
            break;
    }
}

//hàm reset lại tank và màn chơi
static void reset_game(){
    game_over_delay_counter = 0;
    isGameOver = false;
    isPaused = false;
    isVictory = false;
    my_tank.reset();
    my_score.reset();
    my_house.reset();
    my_tree.reset();
    my_enemy.reset();
    my_trap.reset();
    my_mountain.reset();
    my_boss.reset(); // Reset trạng thái Boss
    
}

//hàm save score vào eeprom
static void update_top_scores(uint32_t new_score) {
    uint32_t top[3];
    // 1. Đọc 3 mức điểm hiện tại từ EEPROM
    eeprom_read(0x00, (uint8_t*)&top[0], 4);
    eeprom_read(0x04, (uint8_t*)&top[1], 4);
    eeprom_read(0x08, (uint8_t*)&top[2], 4);

    // 2. So sánh và sắp xếp lại
    if (new_score > top[0]) {
        top[2] = top[1];
        top[1] = top[0];
        top[0] = new_score;
    } else if (new_score > top[1]) {
        top[2] = top[1];
        top[1] = new_score;
    } else if (new_score > top[2]) {
        top[2] = new_score;
    } else {
        return; // Không đủ điểm vào top, không cần ghi EEPROM
    }

    // 3. Ghi lại bảng điểm mới vào EEPROM
    eeprom_write(0x00, (uint8_t*)&top[0], 4);
    eeprom_write(0x04, (uint8_t*)&top[1], 4);
    eeprom_write(0x08, (uint8_t*)&top[2], 4);
    APP_DBG("EEPROM: New High Score Saved!\n");
}
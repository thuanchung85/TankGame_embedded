#include "boss_object.h"


// 1. Khởi tạo vùng nhớ tĩnh cho Boss
boss_t static_boss;

void task_boss_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case BOSS_SETUP_SIG:
        case BOSS_RESET_SIG: {
           static_boss.x = 130;        
            static_boss.y = 14;
            static_boss.max_hp = 10;
            static_boss.hp = static_boss.max_hp;
            static_boss.is_active = false;
            static_boss.is_exploding = false;
            static_boss.explosion_timer = 0;
            static_boss.move_dir = 1;
            static_boss.move_dir_x = 1; // <-- Khởi tạo hướng X ban đầu
            static_boss.isDie = false;
            static_boss.fire_cooldown_counter = 0;
            
            static_boss.rocket.is_active = false;
            static_boss.rocket.x = 0;
            static_boss.rocket.y = 0;
            static_boss.rocket.hp = 2;
        } break;

        case BOSS_SPAWN_SIG: {
           if (!static_boss.is_active && !static_boss.is_exploding && !static_boss.isDie) {
                // SỬA TẠI ĐÂY: Khởi tạo trực tiếp các biến số cấu hình ban đầu
                static_boss.x = 130;        
                static_boss.y = 14;
                static_boss.max_hp = 10;
                static_boss.hp = static_boss.max_hp;
                static_boss.move_dir = 1;
                static_boss.move_dir_x = 1; // <-- Khởi tạo hướng X ban đầu
                static_boss.fire_cooldown_counter = 0;
                static_boss.rocket.is_active = false;
                
                // Sau đó kích hoạt cờ hoạt động lên
                static_boss.is_active = true;
                
                APP_DBG(">> BOSS WARNING: SPAWNING! <<\n");
            }
        } break;

        case BOSS_HIT_SIG: {
           if (!static_boss.is_active) break;

            uint8_t damage = 1; // Mặc định nếu tin nhắn lỗi/không có data thì tính là 1 dame

            // Kiểm tra an toàn: Chỉ đọc dữ liệu nếu tin nhắn này thực sự có payload
            uint8_t* msg_data = get_data_common_msg(msg); 
            if (msg_data != nullptr && msg_data != NULL) {
                damage = msg_data[0]; 
            }

            static_boss.hp -= damage;
            APP_DBG(">> BOSS HIT! HP: %d/%d\n", static_boss.hp, static_boss.max_hp);

            if (static_boss.hp <= 0) {
                static_boss.is_active = false;
                static_boss.is_exploding = true;
                static_boss.explosion_timer = 0;
                APP_DBG(">> BOSS DESTROYED! <<\n");

                // =========================================================
                // TẶNG REWARD 100 ĐIỂM CHO LUONG TẠI ĐÂY:
                // =========================================================
                uint8_t boss_score_bonus = 100; // Khai báo phần thưởng 100 điểm
                task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &boss_score_bonus, sizeof(uint8_t));
            }
        } break;

        case BOSS_UPDATE_SIG: {
            // --- Logic khi Boss đang nổ ---
            if (static_boss.is_exploding) {
                static_boss.explosion_timer++;
                if (static_boss.explosion_timer > 15) {      
                    static_boss.is_exploding = false;
                    static_boss.isDie = true;
                }
                break; // Đang nổ thì không di chuyển hay bắn phá gì nữa
            }

            if (static_boss.isDie || !static_boss.is_active) break; 

            // --- AI Di chuyển của Boss (Lướt sóng Zig-zag toàn diện) ---
            
            // 1. Giai đoạn 1: Khi mới xuất hiện ở X=130, bay từ từ vào màn hình cho tới khi chạm mốc X=90
            if (static_boss.x > 90 && static_boss.move_dir_x == 1) {
                static_boss.x -= 1; 
            } 
            // 2. Giai đoạn 2: Khi đã vào trận, kích hoạt bay Zig-zag tới lui + lên xuống
            else {
                // --- Xử lý trục Y (Lên / Xuống) ---
                static_boss.y += static_boss.move_dir;
                if (static_boss.y <= 2 || static_boss.y >= 26) { 
                    static_boss.move_dir = -static_boss.move_dir; // Đụng biên trên/dưới thì đổi hướng
                }

                // --- Xử lý trục X (Tới / Lui biến ảo) ---
                // move_dir_x = 1 nghĩa là Boss đang lùi về bên phải, move_dir_x = -1 là đang tiến về bên trái
                static_boss.x += static_boss.move_dir_x; 
                
                // Đặt giới hạn phạm vi bay cho trục X (Từ X=40 đến X=90)
                if (static_boss.x <= 40) {
                    static_boss.move_dir_x = 1;  // Đụng cận trái -> Lùi về bên phải
                } 
                else if (static_boss.x >= 90) {
                    static_boss.move_dir_x = -1; // Đụng cận phải -> Tiến về bên trái
                }
            }

            // --- Logic Cooldown nạp đạn và Bắn Rocket ---
            if (!static_boss.rocket.is_active) {
                static_boss.fire_cooldown_counter++;
                if (static_boss.fire_cooldown_counter >= 68) { 
                    if (rand() % 2 == 0) { 
                        static_boss.rocket.is_active = true;
                        static_boss.rocket.x = static_boss.x; // Đạn bay sang trái nên xuất phát lệch trái Boss tí
                        static_boss.rocket.y = static_boss.y + 12;
                        static_boss.rocket.hp = 2;
                        // BUZZER_PlaySound(BUZZER_SOUND_FIRECRACKER);
                        APP_DBG(">> BOSS FIRED ROCKET! <<\n");
                    }
                    static_boss.fire_cooldown_counter = 0;
                }
            }

            // --- Cập nhật vị trí Rocket đang bay ---
            if (static_boss.rocket.is_active) {
                static_boss.rocket.x -= 1; // Rocket bay liên tục sang bên trái hướng về phía xe tăng
                if (static_boss.rocket.x < -17) { 
                    static_boss.rocket.is_active = false; // Bay ra khỏi màn hình thì tắt
                }
            }
        } break;

        default:
            break;
    }
}

void boss_draw() {
    // 1. Vẽ quả Rocket của Boss nếu nó đang kích hoạt
    if (static_boss.rocket.is_active) {
        view_render.drawBitmap(static_boss.rocket.x, static_boss.rocket.y, bitmap_boss_rocket, 17, 11, WHITE);
    }

    // 2. Vẽ thân hình Boss và thanh máu HP
    if (static_boss.is_active) {
        view_render.drawBitmap(static_boss.x, static_boss.y, bitmap_boss, 60, 36, WHITE);
        
        // Hiển thị chữ BOSS và khung HP bar cố định trên màn hình
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(20, 0); 
        view_render.print("BOSS"); 
        view_render.drawRect(52, 2, 50, 3, WHITE); 
        
        // Tính toán độ rộng thanh máu: (Máu hiện tại * 50 pixel) / Máu tối đa
        if (static_boss.hp > 0) {
            int8_t hp_bar_width = (static_boss.hp * 50) / static_boss.max_hp;
            view_render.fillRect(52, 2, hp_bar_width, 3, WHITE);
        }
    } 
    // 3. Vẽ hiệu ứng nổ hoành tráng khi Boss cạn máu
    else if (static_boss.is_exploding) {
        view_render.drawCircle(static_boss.x + 30, static_boss.y + 18, static_boss.explosion_timer * 2, WHITE);
        
        if (static_boss.explosion_timer < 3) {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum, 28, 20, WHITE);
        } else if (static_boss.explosion_timer < 6) {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum3, 30, 31, WHITE);
        }

        if (static_boss.explosion_timer < 3) {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum, 28, 20, WHITE);
        } else if (static_boss.explosion_timer < 6) {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum3, 30, 31, WHITE);
        }
    }
}
#include "minigun_bullet_object.h"

 minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];

void task_minigun_bullet_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case MINIGUN_BULLET_SETUP_SIG:
        case MINIGUN_BULLET_RESET_SIG: {
            // Xóa sạch toàn bộ mảng đạn về trạng thái nghỉ
            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                minigun_pool[i].is_active = false;
                minigun_pool[i].x = 0;
                minigun_pool[i].y = 0;
                minigun_pool[i].targetY = 0;
            }
        } break;

      case MINIGUN_BULLET_FIRE_SIG: {
            extern tank_t static_tank;
            extern enemy_t static_enemy;
            extern boss_t static_boss; // <-- THÊM EXTERN ĐỂ TRUY CẬP RADA BOSS

            // KIẾM VIÊN ĐẠN ĐANG RẢNH TRONG HỒ (POOL) ĐỂ BẮN
            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                if (!minigun_pool[i].is_active) { 
                    minigun_pool[i].x = static_tank.x + 18; 
                    minigun_pool[i].y = 31;                

                    // ====================================================
                    // HỆ THỐNG RADA DẪN ĐƯỜNG ĐA TẦNG NÂNG CẤP CỦA LUONG:
                    // ====================================================
                    
                    // TẦNG 1: Ưu tiên tối cao - Khóa mục tiêu vào quả Rocket của Boss nếu nó đang bay nguy hiểm
                    if (static_boss.is_active && static_boss.rocket.is_active) {
                        // Thêm 5 pixel để đạn nhắm thẳng vào tâm trục Y của quả Rocket (17x11)
                        minigun_pool[i].targetY = static_boss.rocket.y + 5; 
                    }
                     // TẦNG 2: Ưu tiên tối cao - Khóa mục tiêu vào quả cannon của Boss2 nếu nó đang bay nguy hiểm
                    else if (static_boss2.is_active && static_boss2.cannon_bullet.is_active) {
                        minigun_pool[i].targetY = static_boss2.cannon_bullet.y + 5; 
                    }


                    // TẦNG 2: Nếu không có Rocket, khóa thẳng vào thân hình Boss khổng lồ để xả dame
                    else if (static_boss.is_active && !static_boss.is_exploding) {
                        // Nhắm vào khoảng giữa thân Boss (36 pixel chiều cao -> cộng thêm 18)
                        minigun_pool[i].targetY = static_boss.y + 18;
                    }
                    // TẦNG 3: Nếu không có Boss, quay về chế độ phòng thủ máy bay thường như cũ
                    else if (static_enemy.enemy_type == 1 && !static_enemy.isExploding) {
                        minigun_pool[i].targetY = 5; // Khóa mục tiêu tầm cao máy bay thường
                    } 
                    // TẦNG CỐI: Chiến trường trống trải, bắn chéo lên trời giải trí
                    else {
                        minigun_pool[i].targetY = -10; 
                    }

                    minigun_pool[i].is_active = true;
                    break; 
                }
            }
        } break;

        case MINIGUN_BULLET_UPDATE_SIG: {
            for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
                if (minigun_pool[i].is_active) {
                    // 1. Tiến tới theo trục X
                    minigun_pool[i].x += 2; 

                    // 2. Tự động bẻ lái bám đuổi theo trục Y
                    if (minigun_pool[i].y > minigun_pool[i].targetY) {
                        minigun_pool[i].y -= 1; 
                    } 
                    else if (minigun_pool[i].y < minigun_pool[i].targetY) {
                        minigun_pool[i].y += 1; 
                    }

                    // 3. Kiểm tra biên an toàn cho từng viên độc lập
                    // Đạn bay chéo lên trời sẽ chạm biên y < 0 trước, lệnh này sẽ tắt đạn cực kỳ an toàn
                    if (minigun_pool[i].x >= 124 || minigun_pool[i].y < 0 || minigun_pool[i].y > 60) {
                        minigun_pool[i].is_active = false;
                    }
                }
            }
        } break;

        default:
            break;
    }
}

void minigun_bullet_draw() {
   // VẼ TẤT CẢ CÁC VIÊN ĐẠN ĐANG HOẠT ĐỘNG LÊN MÀN HÌNH
    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
        if (minigun_pool[i].is_active) {
            view_render.drawBitmap(minigun_pool[i].x, minigun_pool[i].y, bitmap_myTank_gun_bullet, 2, 1, WHITE);
        }
    }
}
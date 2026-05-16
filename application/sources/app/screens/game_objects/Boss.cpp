#include "Boss.h"

// Định nghĩa Constructor
Boss::Boss() {
    reset();
}

// Hàm để reset kẻ địch khi nó chết hoặc đi hết màn hình
void Boss::reset() {
    x = 130;        // Xuất hiện từ ngoài rìa phải màn hình
    y = 14;         // Vị trí Y trung tâm (màn hình cao 64, boss cao 36 -> (64-36)/2 = 14)
    max_hp = 10;    // Cho Boss ăn 10 phát đạn mới nổ cho "trâu"
    hp = max_hp;
    is_active = false;
    is_exploding = false;
    explosion_timer = 0;
    move_dir = 1;
}

void Boss::spawn() {
    if (!is_active && !is_exploding) {
        reset();
        is_active = true;
    }
}

void Boss::lose_hp(int8_t damage) {
    if (!is_active) return;
    hp -= damage;
    if (hp <= 0) {
        is_active = false;
        is_exploding = true;
        explosion_timer = 0;
    }
}

// Hàm cập nhật trạng thái 
void Boss::update() {
    if (is_exploding) {
        explosion_timer++;
        if (explosion_timer > 15) { // Hiệu ứng nổ diễn ra trong 15 frame
            is_exploding = false;
        }
        return;
    }

    if (!is_active) return;

    // 1. Logic đi vào màn hình: Từ x=130 chạy vào dừng lại ở x=65 (nửa màn hình bên phải)
    if (x > 65) {
        x -= 1; 
    } else {
        // 2. Khi đã vào vị trí, Boss sẽ bay lên bay xuống để né đạn và làm khó người chơi
        y += move_dir;
        if (y <= 2 || y >= 26) { // Giới hạn biên trên và biên dưới để không lệch màn hình
            move_dir = -move_dir; // Đổi hướng
        }
    }
}

// Hàm vẽ Enemy
void Boss::draw() {
    if (is_active) {
        // Vẽ Boss
        view_render.drawBitmap(x, y, bitmap_boss, 60, 36, WHITE);
        
        // 2. Vẽ giao diện thanh máu cố định trên HUD (Top Screen)
        view_render.setTextSize(1);
        view_render.setCursor(20, 0); // Đặt chữ BOSS ở x=20, y=0 cho thẳng hàng với thanh máu
        view_render.print("BOSS:"); 
        
        // Vẽ khung thanh máu (Dịch sang x=52 để không đè lên chữ "BOSS:")
        view_render.drawRect(52, 2, 50, 3, WHITE); 
        
        // Tính toán độ dài máu còn lại
        int8_t hp_bar_width = (hp * 50) / max_hp;
        // Vẽ ruột thanh máu fill đầy tương ứng với HP
        view_render.fillRect(52, 2, hp_bar_width, 3, WHITE);

    } 
    else if (is_exploding) {
        // Vẽ hiệu ứng nổ (Vẽ vài vòng tròn đồng tâm to dần hoặc dùng hàm nổ có sẵn)
        view_render.drawCircle(x + 30, y + 18, explosion_timer * 2, WHITE);
    }
}

bool Boss::check_collision(int16_t bx, int16_t by, int8_t bw, int8_t bh) {
    if (!is_active) return false;
    // Check va chạm giữa hộp 60x36 của Boss và đạn (bx, by, bw, bh)
    return (bx < x + 60 && bx + bw > x && by < y + 36 && by + bh > y);
}
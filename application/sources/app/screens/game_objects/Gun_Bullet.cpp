#include "Gun_Bullet.h"

// Định nghĩa Constructor
Gun_Bullet::Gun_Bullet() {
   is_active = false;
}

void Gun_Bullet::fire(short startX, short startY) {
        x = startX;
        y = startY;
        is_active = true;
    }

// Hàm cập nhật trạng thái 
void Gun_Bullet::update() {
    if (is_active) {
        x += 2; // Tốc độ đạn bay (có thể chỉnh nhanh chậm ở đây)
        if (x > 124) is_active = false; // Bay ra khỏi màn hình thì tắt
    }
}


// Hàm vẽ 
void Gun_Bullet::draw() {
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_myTank_gun_bullet, 2, 1, WHITE);
    }
}


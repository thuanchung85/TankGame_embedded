#include "Gun_Bullet.h"

// Định nghĩa Constructor
Gun_Bullet::Gun_Bullet() {
   is_active = false;
}

void Gun_Bullet::fire(short startX, short startY, short destY) {
    x = startX;
    y = startY;
    targetY = destY; // Máy bay đang ở y = 5
    is_active = true;
}

// Hàm cập nhật trạng thái 
void Gun_Bullet::update() {
    if (is_active) {
        x += 3; // Tăng tốc độ đạn Gun nhanh hơn đạn Canon một chút

        // Logic đuổi theo mục tiêu theo trục Y
        if (y > targetY) {
            y -= 1; // Nếu đạn đang ở dưới mục tiêu, nhích lên 1 pixel
        } else if (y < targetY) {
            y += 1; // Nếu đạn đang ở trên mục tiêu, nhích xuống 1 pixel
        }

        if (x > 124 || y < 0 || y > 60) is_active = false; 
    }
}


// Hàm vẽ 
void Gun_Bullet::draw() {
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_myTank_gun_bullet, 2, 1, WHITE);
    }
}


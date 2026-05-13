#include "Trap.h"

// Định nghĩa Constructor
Trap::Trap() {
    x = 0 + (rand() % 100);
    y = -20;
}

// Hàm cập nhật trạng thái 
void Trap::update() {
    y++;
    if (y > 65) {
        x = 0 + (rand() % 100);
        y = -20;
    }        
}

// Hàm vẽ Trap chính là quả rocket
void Trap::draw() {
           
    view_render.drawBitmap(x, y, bitmap_enemy_rocket, 11, 17, WHITE);
       
}

//hàm check va chạm với tank
bool Trap::checkCollisionWithTank(short tankX, short tankY, short tankW, short tankH) {
    int8_t rocketW = 5; // Kích thước bitmap_enemy_rocket
    int8_t rocketH = 8;

    if (x < tankX + tankW/2 &&
        x + rocketW > tankX &&
        y < tankY + tankH &&
        y + rocketH > tankY) {
        return true; 
    }
    return false;
}
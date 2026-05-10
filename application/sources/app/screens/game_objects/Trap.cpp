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






#include "Enemy_Tank.h"

// Định nghĩa Constructor
Enemy_Tank::Enemy_Tank() {
    x = 130 + (rand() % 100);
    y = 33 + (rand() % 10);
}

// Hàm cập nhật trạng thái 
void Enemy_Tank::update() {

    //scroll trên mặt đất, cho nó di chuyển từ phải sang trái
    x--;
    // Nếu đã trôi hết qua trái màn hình
    if (x < -35) {
        x = 130 + (rand() % 100);
        y = 33 + (rand() % 10); // Reset về bên phải để xuất hiện lại
    }
   
}

// Hàm vẽ Enemy_Tank
void Enemy_Tank::draw() {
    view_render.drawBitmap(x, y, bitmap_enemy_tank, 25, 21, WHITE);
}






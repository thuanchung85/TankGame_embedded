#include "HP_point.h"

// Định nghĩa Constructor
HP_point::HP_point() {
    reset();
}
void HP_point::reset(){
    hpPoint = 1; // Mặc định là hp point = 1
}

// Hàm cập nhật trạng thái 
void HP_point::update() {
    
}

// Trong file HP_point.h thêm:
void HP_point::subHP() {
    if (hpPoint > 0) hpPoint--;
}
//nếu hết HP thì tank die
bool HP_point::isDead() {
    return hpPoint <= 0;
}

// Hàm vẽ 
void HP_point::draw() {
    // Vẽ số lượng trái tim tương ứng với lượng máu hiện có
    for (int i = 0; i < hpPoint; i++) {
        // Mỗi trái tim cách nhau 12 pixel (10px chiều rộng + 2px khoảng cách)
        view_render.drawBitmap(1 + (i * 12), 1, bitmap_HP, 10, 8, WHITE);
    }
    //view_render.drawBitmap(1, 1, bitmap_HP, 10, 8, WHITE);

}


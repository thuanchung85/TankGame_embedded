#ifndef TANK_H
#define TANK_H

#include "../bitmap_data/screens_bitmap.h"
#include "Canon_Bullet.h"

class Tank {
public:
    int8_t x;//game play chỉ có 2 nut bấm cho xe tank tới lui né tránh đạn
    int8_t speed;
    int8_t frame_count;

    Canon_Bullet my_bullets; // Cho phép tối đa 3 viên đạn trên màn hình
    
    Tank();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ

    void tank_fire(); //khai báo hàm tank bắn 
};

#endif
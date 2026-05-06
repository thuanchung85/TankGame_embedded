#ifndef TANK_H
#define TANK_H

#include "../bitmap_data/screens_bitmap.h"

class Tank {
public:
    int8_t x;//game play chỉ có 2 nut bấm cho xe tank tới lui né tránh đạn
    int8_t speed;
    
    Tank();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
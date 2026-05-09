#ifndef ENEMY_TANK_H
#define ENEMY_TANK_H

#include "../bitmap_data/screens_bitmap.h"

class Enemy_Tank {
public:
    short x;
    int8_t y;
    int8_t frame_count;
 
    Enemy_Tank();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ

};

#endif
#ifndef ENEMY_H
#define ENEMY_H

#include "../bitmap_data/screens_bitmap.h"

class Enemy {
public:
    short x;
    int8_t frame_count;
    int8_t enemy_type;
 
    Enemy();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ

};

#endif
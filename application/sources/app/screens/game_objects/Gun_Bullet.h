#ifndef GUN_BULLET_H
#define GUN_BULLET_H

#include "../bitmap_data/screens_bitmap.h"

class Gun_Bullet {
public:
    int8_t x, y;
    bool is_active; // Đạn có đang bay không?
    
    Gun_Bullet();         // Khai báo Constructor

    void fire(short startX, short startY);//hàm khai hỏa
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
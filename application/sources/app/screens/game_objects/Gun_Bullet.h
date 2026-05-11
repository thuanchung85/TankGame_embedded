#ifndef GUN_BULLET_H
#define GUN_BULLET_H

#include "../bitmap_data/screens_bitmap.h"

class Gun_Bullet {
public:
    bool is_active; // Đạn có đang bay không?
    int8_t x, y;
    short targetY; // Lưu tọa độ Y của máy bay lúc bắt đầu bắn
    
    Gun_Bullet();         // Khai báo Constructor
    
    void fire(short startX, short startY, short destY);
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
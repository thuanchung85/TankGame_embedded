#ifndef ENEMY_H
#define ENEMY_H

#include "../bitmap_data/screens_bitmap.h"

class Enemy {
public:
    short x;
    bool isExploding;    // Đang nổ hay không
    short explosionTimer;  // Bộ đếm thời gian nổ
    int8_t enemy_type;
 
    Enemy();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
    bool checkCollision(short bulletX, short bulletY, short bulletW, short bulletH);
};

#endif
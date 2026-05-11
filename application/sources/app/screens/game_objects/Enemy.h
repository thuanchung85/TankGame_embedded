#ifndef ENEMY_H
#define ENEMY_H

#include "../bitmap_data/screens_bitmap.h"

class Enemy {
public:
    bool isExploding;    // Đang nổ hay không
    int8_t enemy_type;
    int8_t hp;              // Máu hiện tại của kẻ địch
    int8_t explosionTimer;  // Bộ đếm thời gian nổ
    short x;
    
    Enemy();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
    bool checkCollision(short bulletX, short bulletY, short bulletW, short bulletH);
    void reset();
};

#endif
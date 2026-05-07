#ifndef HP_POINT_H
#define HP_POINT_H

#include "../bitmap_data/screens_bitmap.h"

class HP_point {
public: 
    int8_t hpPoint;
 
    HP_point();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw(); // Khai báo hàm vẽ
    void subHP();   //khai báo hàm trừ máu HP
    bool isDead();//check tank có hết HP
};

#endif
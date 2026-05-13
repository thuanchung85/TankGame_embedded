#ifndef TRAP_H
#define TRAP_H

#include "../bitmap_data/screens_bitmap.h"

class Trap {
public:
    short x,y;
    
    Trap();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ

    bool checkCollisionWithTank(short tankX, short tankY, short tankW, short tankH); //check va chạm với tank

};

#endif
#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "../bitmap_data/screens_bitmap.h"

class Mountain {
public:
    short x;//vị trí 
    int8_t frame_count;
    
    Mountain();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
    void reset();
};

#endif
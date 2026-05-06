#ifndef GROUND_H
#define GROUND_H

#include "../bitmap_data/screens_bitmap.h"

class Ground {
public:
    int8_t scroll_offset;
    
    Ground();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
#ifndef HOUSE_H
#define HOUSE_H

#include "../bitmap_data/screens_bitmap.h"

class House {
public:
    short x;//vị trí 
    int8_t frame_count;
    int8_t house_type; // Biến để lưu kiểu nhà hiện tại (0 đến 3)
 
    House();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
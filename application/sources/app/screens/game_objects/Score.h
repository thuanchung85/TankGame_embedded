#ifndef SCORE_H
#define SCORE_H

#include "../bitmap_data/screens_bitmap.h"

class Score {
public: 
    int8_t current_score;
    unsigned short frame_count;//cứ sau 1 lượng frame thí cộng 1 điểm
 
    Score();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw(); // Khai báo hàm vẽ
    void add();
};

#endif
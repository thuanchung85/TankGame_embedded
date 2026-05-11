#ifndef SCORE_H
#define SCORE_H

#include "../bitmap_data/screens_bitmap.h"

class Score {
public: 
    unsigned short current_score;
 
    Score();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw(); // Khai báo hàm vẽ
    void add();
};

#endif
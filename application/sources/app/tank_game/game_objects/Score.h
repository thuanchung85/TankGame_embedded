#ifndef SCORE_H
#define SCORE_H

#include "../bitmap_data/screens_bitmap.h"

class Score {
public: 
    uint32_t current_score;
 
    Score();         
    void update();  
    void draw(); 
    void add(int value);
    void reset();
};

#endif
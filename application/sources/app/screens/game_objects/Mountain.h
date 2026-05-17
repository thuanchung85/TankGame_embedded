#ifndef MOUNTAIN_H
#define MOUNTAIN_H

#include "../bitmap_data/screens_bitmap.h"

class Mountain {
public:
    short x;
    int8_t frame_count;
    
    Mountain();         
    void update();  
    void draw();    
    void reset();
};

#endif
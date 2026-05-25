#ifndef GROUND_H
#define GROUND_H

#include "../bitmap_data/screens_bitmap.h"

class Ground {
public:
    int8_t scroll_offset;
    
    Ground();         
    void update();  
    void draw();    
};

#endif
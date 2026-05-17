#ifndef HOUSE_H
#define HOUSE_H

#include "../bitmap_data/screens_bitmap.h"

class House {
public:
    short x;
    int8_t frame_count;
    int8_t house_type; 
 
    House();         
    void update();  
    void draw();    
    void reset();
};

#endif
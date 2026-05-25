#ifndef HP_POINT_H
#define HP_POINT_H

#include "../bitmap_data/screens_bitmap.h"

class HP_point {
public: 
    int8_t hpPoint;
 
    HP_point();        
    void update();  
    void draw(); 
    void subHP();   
    bool isDead();
    void reset();
};

#endif
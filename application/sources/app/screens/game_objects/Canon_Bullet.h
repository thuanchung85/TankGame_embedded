#ifndef CANON_BULLET_H
#define CANON_BULLET_H

#include "../bitmap_data/screens_bitmap.h"

class Canon_Bullet {
public:
    int8_t x, y;
    bool is_active; 
    
    Canon_Bullet();         

    void fire(short startX, short startY);
    void update();  
    void draw();    
};

#endif
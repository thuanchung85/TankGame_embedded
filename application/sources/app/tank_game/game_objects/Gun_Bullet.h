#ifndef GUN_BULLET_H
#define GUN_BULLET_H

#include "../bitmap_data/screens_bitmap.h"

class Gun_Bullet {
public:
    bool is_active; 
    int8_t x, y;
    short targetY; 
    
    Gun_Bullet();         
    
    void fire(short startX, short startY, short destY);
    void update();  
    void draw();   
};

#endif
#ifndef CANNON_BULLET_H
#define CANNON_BULLET_H

#include "../bitmap_data/screens_bitmap.h"

class Cannon_Bullet {
public:
    int8_t x, y;
    bool is_active; 
    
    Cannon_Bullet();         

    void fire(short startX, short startY);
    void update();  
    void draw();    
};

#endif
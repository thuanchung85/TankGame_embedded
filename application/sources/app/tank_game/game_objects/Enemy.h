#ifndef ENEMY_H
#define ENEMY_H

#include "../bitmap_data/screens_bitmap.h"

class Enemy {
public:
    bool isExploding;    
    int8_t enemy_type;
    int8_t hp;              
    int8_t explosionTimer;  
    short x;
    
    Enemy();         
    void update();  
    void draw();    
    bool checkCollision(short bulletX, short bulletY, short bulletW, short bulletH);
    void reset();
};

#endif
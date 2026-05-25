#ifndef TRAP_H
#define TRAP_H

#include "../bitmap_data/screens_bitmap.h"

class Trap {
public:
    short x,y;
    
    Trap();         
    void update();  
    void draw();    
    void reset();
    bool checkCollisionWithTank(short tankX, short tankY, short tankW, short tankH); 

};

#endif
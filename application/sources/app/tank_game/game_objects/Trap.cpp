#include "Trap.h"

Trap::Trap() {
   reset();
}
void Trap::reset(){
    x = 0 + (rand() % 100);
    y = -20;
}

void Trap::update() {
    y++;
    if (y > 65) {
        x = 0 + (rand() % 100);
        y = -20;
    }        
}

void Trap::draw() {
           
    view_render.drawBitmap(x, y, bitmap_enemy_rocket, 11, 17, WHITE);
       
}

bool Trap::checkCollisionWithTank(short tankX, short tankY, short tankW, short tankH) {
    int8_t rocketW = 5; 
    int8_t rocketH = 8;

    if (x < tankX + tankW/2 &&
        x + rocketW > tankX &&
        y < tankY + tankH &&
        y + rocketH > tankY) {
        return true; 
    }
    return false;
}
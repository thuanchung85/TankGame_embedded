#include "HP_point.h"

HP_point::HP_point() {
    reset();
}
void HP_point::reset(){
    hpPoint = 1; 
}

void HP_point::update() {
    
}

void HP_point::subHP() {
    if (hpPoint > 0) hpPoint--;
}

bool HP_point::isDead() {
    return hpPoint <= 0;
}

void HP_point::draw() {
    for (int i = 0; i < hpPoint; i++) {
        view_render.drawBitmap(1 + (i * 12), 1, bitmap_HP, 10, 8, WHITE);
    }
}


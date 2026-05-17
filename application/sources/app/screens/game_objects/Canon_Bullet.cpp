#include "Canon_Bullet.h"

Canon_Bullet::Canon_Bullet() {
   is_active = false;
}

void Canon_Bullet::fire(short startX, short startY) {
        x = startX;
        y = startY;
        is_active = true;
    }

void Canon_Bullet::update() {
    if (is_active) {
        x += 2; 
        if (x > 124) is_active = false; 
    }
}

void Canon_Bullet::draw() {
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_myTank_canon_bullet, 5, 3, WHITE);
    }
}


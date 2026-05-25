#include "Cannon_Bullet.h"

Cannon_Bullet::Cannon_Bullet() {
   is_active = false;
}

void Cannon_Bullet::fire(short startX, short startY) {
        x = startX;
        y = startY;
        is_active = true;
    }

void Cannon_Bullet::update() {
    if (is_active) {
        x += 2; 
        if (x > 124) is_active = false; 
    }
}

void Cannon_Bullet::draw() {
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_myTank_cannon_bullet, 5, 3, WHITE);
    }
}


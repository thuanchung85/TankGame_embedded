#include "Gun_Bullet.h"

Gun_Bullet::Gun_Bullet() {
   is_active = false;
}

void Gun_Bullet::fire(short startX, short startY, short destY) {
    x = startX;
    y = startY;
    targetY = destY; 
    is_active = true;
}

void Gun_Bullet::update() {
    if (is_active) {
        x += 3; 

        if (y > targetY) {
            y -= 1; 
        } else if (y < targetY) {
            y += 1; 
        }

        if (x > 124 || y < 0 || y > 60) is_active = false; 
    }
}

void Gun_Bullet::draw() {
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_myTank_gun_bullet, 2, 1, WHITE);
    }
}


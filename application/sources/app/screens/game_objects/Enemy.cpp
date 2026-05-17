#include "Enemy.h"

Enemy::Enemy() {
    reset();
}

void Enemy::reset() {
    isExploding = false;
    explosionTimer = 0;
    enemy_type = rand() % 4;
    if (enemy_type == 0 || enemy_type == 1) { 
        hp = 2;
    } else {               
        hp = 1;
    }
    x = 150 + (rand() % 100);
}

void Enemy::update() {

    if (isExploding) 
    {
        explosionTimer++;
        if (explosionTimer > 9) {
            reset(); 
        }
        return;
    }
    else 
    {
        if(enemy_type == 3 || enemy_type == 1)
        {
            x -= 2;
        }
        else
        {
            x--;
        }
        if (x < -35) {
            reset();
        }
    }   
   
}

void Enemy::draw() {

    if (isExploding) {
        if(enemy_type != 1)//for tank, mine, troop at y = 30
        {
            if (explosionTimer < 3) {
                view_render.drawBitmap(x, 30, bitmap_bum, 28, 20, WHITE);
            } else if (explosionTimer < 6) {
                view_render.drawBitmap(x, 25, bitmap_bum2, 30, 26, WHITE);
            } else {
                view_render.drawBitmap(x, 20, bitmap_bum3, 30, 31, WHITE);
            }
        }
        else //for air at y = 5
        {
            if (explosionTimer < 3) {
                view_render.drawBitmap(x, 5, bitmap_bum, 28, 20, WHITE);
            } else if (explosionTimer < 6) {
                view_render.drawBitmap(x, 5, bitmap_bum2, 30, 26, WHITE);
            } else {
                view_render.drawBitmap(x, 5, bitmap_bum3, 30, 31, WHITE);
            }
        }

    } 
    else 
    {
        switch (enemy_type)
        {
            case 0:// tank
                    view_render.drawBitmap(x, 33, bitmap_enemy_tank, 25, 21, WHITE);
                    break;

            case 1:// air
                    view_render.drawBitmap(x, 5, bitmap_enemy_air_plane, 25, 21, WHITE);
                    break;

            case 2:// mine
                    view_render.drawBitmap(x, 33, bitmap_enemy_mine, 22, 21, WHITE);
                    break;

            case 3:// troop
                view_render.drawBitmap(x, 34, bitmap_enemy_troop, 15, 21, WHITE);
                break;
        }
    }
}

bool Enemy::checkCollision(short bulletX, short bulletY, short bulletW, short bulletH) {
    short enemyW, enemyH, enemyY;
    
    switch (enemy_type) {
        case 0: enemyW = 25; enemyH = 21; enemyY = 33; break; // Tank
        case 1: enemyW = 25; enemyH = 21; enemyY = 5;  break; // Air
        case 2: enemyW = 22; enemyH = 21; enemyY = 33; break; // Mine
        case 3: enemyW = 15; enemyH = 21; enemyY = 34; break; // Troop
        default: return false;
    }

    if (bulletX < x + enemyW &&
        bulletX + bulletW > x &&
        bulletY < enemyY + enemyH &&
        bulletY + bulletH > enemyY) {
        return true; 
    }
    
    return false;
}
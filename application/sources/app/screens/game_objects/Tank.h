#ifndef TANK_H
#define TANK_H

#include "../bitmap_data/screens_bitmap.h"
#include "Cannon_Bullet.h"
#include "Gun_Bullet.h"
#include "HP_point.h"

class Tank {
public:
    int8_t x; //x of tank
    int8_t target_x; //x target for moving of tank
    int8_t frame_count;
    int8_t explosion_timer;
    bool is_cannon_reloaded;
    bool isExploding;
    bool isMoving;

    Gun_Bullet my_gun_bullets; // air gun
    Cannon_Bullet my_cannon_bullets; //  cannon
    HP_point myHP;
    
    Tank();         
    void update();  
    void draw();    

    void tank_fire_cannon(); 
    void tank_fire_gun(short enemyY);

    void moveForward(); 
    void moveBackward();

    void lossHP();

    void reset();

    bool checkCollisionWithEnemy(short enemyX, short enemyY, short enemyW, short enemyH);
};

#endif
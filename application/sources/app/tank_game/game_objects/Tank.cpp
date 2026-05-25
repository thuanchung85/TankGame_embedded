#include "Tank.h"
extern "C" {
    #include "../../../../driver/buzzer/buzzer.h"
}

Tank::Tank() {
    reset();
}

void Tank::reset(){
    x = 20;
    target_x = 20;
    isMoving = false;
    isExploding = false;
    is_cannon_reloaded = true;
    explosion_timer = 0;
    is_firing_gun = false;
    myHP.reset();
}

void Tank::update(Enemy& enemy, Boss& boss) 
{
    //if tank alive
    if(!isExploding)
    { 
        //tank move to x target
        if(isMoving == true)
        {
            if (x < target_x) x += 1;
            if (x > target_x) x -= 1;
            if(x == target_x) isMoving = false;
        }
    
        //fire cannon auto reload each 50 frame
        frame_count++;
        if(frame_count >= 50)
        {
            is_cannon_reloaded = true;
            frame_count = 0;
        }

        // =================================================================
        //  (MINIGUN LOGIC) OF TANK
        // =================================================================
        if (is_firing_gun) {
            if (!boss.is_active && enemy.enemy_type == 1 && enemy.x < 120) {
                tank_fire_gun(5); 
            } 
            else if (boss.is_active && boss.rocket.is_active && boss.rocket.x < 120) {
                tank_fire_gun(boss.rocket.y); 
            }
        }

        // update cannon tank
        my_cannon_bullets.update();
        my_gun_bullets.update();

        //HP update
        myHP.update();
    }
    else// if tank die
    {
        explosion_timer++;
        if(explosion_timer > 20) { 
            explosion_timer = 0;
        }
    }
}

void Tank::draw() {
    //if tank die
    if (isExploding) 
    {
        // animation bitmap (bum, bum2, bum3)
        if (explosion_timer < 5) view_render.drawBitmap(x, 30, bitmap_bum, 28, 20, WHITE);
        else if (explosion_timer < 10) view_render.drawBitmap(x, 30, bitmap_bum2, 30, 26, WHITE);
         else if (explosion_timer < 15) view_render.drawBitmap(x, 30, bitmap_bum3, 30, 26, WHITE);
    }
    //if tank alive
    else
    {
        view_render.drawBitmap(x, 30, bitmap_tank, 30, 30, WHITE);//draw tank
        my_cannon_bullets.draw(); //draw cannon bullet
        my_gun_bullets.draw(); //draw auto gun bullet
        myHP.draw(); //draw hp
    }
}

//tank fire cannon
void Tank::tank_fire_cannon() {
   if (!my_cannon_bullets.is_active &&  is_cannon_reloaded) {
        my_cannon_bullets.fire(x + 25, 38); 
        is_cannon_reloaded = false;
        BUZZER_PlaySound(BUZZER_SOUND_CLICK);
    }
}

//tank fire auto air gun
void Tank::tank_fire_gun(short enemyY) {
    if (!my_gun_bullets.is_active) {
        my_gun_bullets.fire(x + 18, 31, enemyY); 
    }
}

//tank move forward
void Tank::moveForward() {
    if(isMoving == false)
    {
        if (x < 100-30) { 
            target_x += 10; 
            isMoving = true;
        }
    }
   
}

//tank move backward
void Tank::moveBackward() {
    if(isMoving == false){
        if (x > 0) { 
            target_x -= 10; 
            isMoving = true;
        }
    }
   
}

//tank check collision
bool Tank::checkCollisionWithEnemy(short enemyX, short enemyY, short enemyW, short enemyH) {
    int8_t tankW = 25; // width tank
    int8_t tankH = 15; // heigh tank
    int8_t tankY = 30; // Y of tank

    if (x < enemyX + enemyW &&
        x + tankW > enemyX &&
        tankY < enemyY + enemyH &&
        tankY + tankH > enemyY) {
        return true; 
    }
    return false;
}

//tank loss hp
void Tank::lossHP(){
    myHP.subHP();
    if(myHP.hpPoint <= 0)
    {
        isExploding = true;
    }
}
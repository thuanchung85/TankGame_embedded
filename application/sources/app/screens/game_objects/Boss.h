#ifndef __BOSS_H__
#define __BOSS_H__

#include "../bitmap_data/screens_bitmap.h"
//  Rocket of Boss
struct BossRocket {
    int8_t hp; // HP of rocket
    int16_t x, y;
    bool is_active;
   
    
    void reset(int8_t _hp) {
        x = 0;
        y = 0;
        hp = _hp;
        is_active = false;
    }
    void lose_hp(int8_t damage) {
        hp -= damage;
        if (hp <= 0) {
            is_active = false; // Rocket die
        }
    }
};

class Boss {
public:
    int16_t x, y;
    int8_t hp;
    int8_t max_hp;
    int8_t move_dir; // move up or down (1 or -1)
    uint8_t explosion_timer;
    bool is_active;
    bool is_exploding;
    bool isDie; 
   
   
    Boss();
    BossRocket rocket;
    uint8_t fire_cooldown_counter; //rocket luanch each 1s

    void spawn();
    void update();
    void draw();
    void reset();
    void lose_hp(int8_t damage);
    bool check_collision(int16_t bullet_x, int16_t bullet_y, int8_t bullet_w, int8_t bullet_h);
};

#endif //__BOSS_H__
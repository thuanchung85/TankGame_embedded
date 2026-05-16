#ifndef __BOSS_H__
#define __BOSS_H__

#include "../bitmap_data/screens_bitmap.h"

class Boss {
public:
    int16_t x, y;
    int8_t hp;
    int8_t max_hp;
    bool is_active;
    bool is_exploding;
    uint8_t explosion_timer;
    int8_t move_dir; // Hướng di chuyển lên/xuống (1 hoặc -1)

    Boss();
    void spawn();
    void update();
    void draw();
    void reset();
    void lose_hp(int8_t damage);
    bool check_collision(int16_t bullet_x, int16_t bullet_y, int8_t bullet_w, int8_t bullet_h);
};

#endif //__BOSS_H__
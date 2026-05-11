#ifndef TANK_H
#define TANK_H

#include "../bitmap_data/screens_bitmap.h"
#include "Canon_Bullet.h"
#include "Gun_Bullet.h"
#include "HP_point.h"

class Tank {
public:
    int8_t x;//game play chỉ có 2 nut bấm cho xe tank tới lui né tránh đạn
    int8_t target_x; // Tọa độ mà tank muốn hướng tới
    bool isMoving;
    int8_t frame_count;

    Gun_Bullet my_gun_bullets;//đạn trên gun
    Canon_Bullet my_canon_bullets; // đạn canon
    HP_point myHP;//chỉ số HP của tank
    
    Tank();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ

    void tank_fire_canon(); //khai báo hàm tank bắn 
    void tank_fire_gun(short enemyY);

    void moveForward(); // Tiến lên
    void moveBackward(); // Lùi lại
};

#endif
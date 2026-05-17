#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "../game_objects/Tank.h"
#include "../game_objects/Enemy.h"
#include "../game_objects/Boss.h"
#include "../game_objects/Trap.h"
#include "../game_objects/Score.h"

class CollisionManager {
public:
    // Hàm tổng quản lý, gọi tất cả các check va chạm bên trong
    static void check_all_collisions(Tank& tank, Enemy& enemy, Boss& boss, Trap& trap, Score& score);

private:
    // Các hàm check va chạm thành phần để code tường minh
    static void check_bullet_with_boss(Tank& tank, Boss& boss, Score& score);
    static void check_tank_with_boss(Tank& tank, Boss& boss);
    static void check_tank_with_enemy(Tank& tank, Enemy& enemy);
    static void check_bullets_with_enemy(Tank& tank, Enemy& enemy, Score& score);
    static void check_trap_with_tank(Tank& tank, Trap& trap);
};

#endif //__COLLISION_MANAGER_H__
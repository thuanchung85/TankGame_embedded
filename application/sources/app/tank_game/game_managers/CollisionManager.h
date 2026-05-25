#ifndef __COLLISION_MANAGER_H__
#define __COLLISION_MANAGER_H__

#include "../game_objects/Tank.h"
#include "../game_objects/Enemy.h"
#include "../game_objects/Boss.h"
#include "../game_objects/Trap.h"
#include "../game_objects/Score.h"

class CollisionManager {
public:
    static void check_all_collisions(Tank& tank, Enemy& enemy, Boss& boss, Trap& trap, Score& score);

private:
    static void check_bullet_with_boss(Tank& tank, Boss& boss, Score& score);
    static void check_tank_with_boss(Tank& tank, Boss& boss);
    static void check_tank_with_enemy(Tank& tank, Enemy& enemy);
    static void check_bullets_with_enemy(Tank& tank, Enemy& enemy, Score& score);
    static void check_trap_with_tank(Tank& tank, Trap& trap);
    static void check_boss_rocket_with_tank(Tank& tank, Boss& boss);
    static void check_cannon_bullet_with_boss_rocket(Tank& tank, Boss& boss);
    static void check_gun_bullet_with_boss_rocket(Tank& tank, Boss& boss);
};

#endif //__COLLISION_MANAGER_H__
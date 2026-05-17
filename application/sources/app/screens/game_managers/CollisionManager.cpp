#include "CollisionManager.h"

void CollisionManager::check_all_collisions(Tank& tank, Enemy& enemy, Boss& boss, Trap& trap, Score& score) {
    check_bullet_with_boss(tank, boss, score);
    check_tank_with_boss(tank, boss);
    check_tank_with_enemy(tank, enemy);
    check_bullets_with_enemy(tank, enemy, score);
    check_trap_with_tank(tank, trap);
}

void CollisionManager::check_bullet_with_boss(Tank& tank, Boss& boss, Score& score) {
    if (tank.my_canon_bullets.is_active && boss.is_active) {
        if (boss.check_collision(tank.my_canon_bullets.x, tank.my_canon_bullets.y, 5, 3)) {
            tank.my_canon_bullets.is_active = false; 
            boss.lose_hp(1);
            BUZZER_PlaySound(BUZZER_SOUND_BANG); 
            
            if (boss.hp <= 0) { 
                score.current_score += 100; 
                BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
            }
        }
    }
}

void CollisionManager::check_tank_with_boss(Tank& tank, Boss& boss) {
    if (boss.is_active && !tank.isExploding) {
        if (boss.check_collision(tank.x, 33, 25, 15)) { 
            tank.lossHP();
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
        }
    }
}

void CollisionManager::check_tank_with_enemy(Tank& tank, Enemy& enemy) {
    if (!enemy.isExploding && enemy.hp > 0) {
        int8_t eY = (enemy.enemy_type == 1) ? 5 : 33; // máy bay y=5, còn lại y=33
        int8_t eW = (enemy.enemy_type == 3) ? 15 : ((enemy.enemy_type == 2) ? 22 : 25);
        int8_t eH = 21;

        if (tank.checkCollisionWithEnemy(enemy.x, eY, eW, eH)) {
            enemy.isExploding = true;
            enemy.explosionTimer = 0;
            tank.lossHP();                    
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION);
        }
    }
}

void CollisionManager::check_bullets_with_enemy(Tank& tank, Enemy& enemy, Score& score) {
    // 1. Check đạn Canon với địch đất (tank, mìn, lính)
    if (tank.my_canon_bullets.is_active && enemy.enemy_type != 1 && enemy.hp > 0) {
        if (enemy.checkCollision(tank.my_canon_bullets.x, tank.my_canon_bullets.y, 5, 3)) {
            tank.my_canon_bullets.is_active = false;                        
            enemy.hp--;
            if (enemy.hp <= 0) {
                enemy.isExploding = true;
                enemy.explosionTimer = 0;
                score.add(); 
                BUZZER_PlaySound(BUZZER_SOUND_BANG); 
            }
        }
    }

    // 2. Check đạn Gun với máy bay (air)
    if (tank.my_gun_bullets.is_active && enemy.enemy_type == 1 && enemy.hp > 0) {
        if (enemy.checkCollision(tank.my_gun_bullets.x, tank.my_gun_bullets.y, 2, 1)) {
            tank.my_gun_bullets.is_active = false;                        
            enemy.hp--;
            if (enemy.hp <= 0) {
                enemy.isExploding = true;
                enemy.explosionTimer = 0;
                score.add(); 
                BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
            }
        }
    }
}

void CollisionManager::check_trap_with_tank(Tank& tank, Trap& trap) {
    if (!tank.isExploding) {
        if (trap.checkCollisionWithTank(tank.x, 30, 25, 15)) {
            tank.lossHP(); 
            trap.y = -20;
            trap.x = rand() % 100;                    
            BUZZER_PlaySound(BUZZER_SOUND_BANG);
        }
    }
}
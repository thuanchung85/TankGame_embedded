#ifndef __BOSS_OBJECT_H__
#define __BOSS_OBJECT_H__

#include <stdint.h>
#include <stdbool.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"
#include "score_object.h"
#include "cannon_bullet_object.h"
#include "tank_object.h"
#include "minigun_bullet_object.h"

struct ak_msg_t;

enum {
    BOSS_SETUP_SIG = 1,
    BOSS_SPAWN_SIG,
    BOSS_UPDATE_SIG,
    BOSS_HIT_SIG,       // Nhận sát thương khi bị đạn trúng
    BOSS_RESET_SIG
};

// Định nghĩa cấu trúc Rocket nằm ngay trong Boss
typedef struct {
    int16_t x;
    int16_t y;
    int8_t hp;
    bool is_active;
} boss_rocket_t;

// Cấu trúc phẳng quản lý Boss
typedef struct {
    int16_t x;
    int16_t y;
    int8_t max_hp;
    int8_t hp;
    int8_t move_dir;
    int8_t move_dir_x; // <-- THÊM DÒNG NÀY: Hướng di chuyển trục X (tới/lui)
    uint8_t explosion_timer;
    uint8_t fire_cooldown_counter;
    bool is_active;
    bool is_exploding;
    bool isDie;
    
    boss_rocket_t rocket; // Đối tượng rocket đi kèm Boss
} boss_t;

extern boss_t static_boss;

#ifdef __cplusplus
extern "C" {
#endif
void check_and_spawn_boss(uint8_t score);

void task_boss_handle(struct ak_msg_t *msg);
void boss_draw();

#ifdef __cplusplus
}
#endif

#endif // __BOSS_OBJECT_H__
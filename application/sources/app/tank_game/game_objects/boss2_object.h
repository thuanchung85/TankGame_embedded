#ifndef __BOSS2_OBJECT_H__
#define __BOSS2_OBJECT_H__

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
    BOSS2_SETUP_SIG = 1,
    BOSS2_SPAWN_SIG,
    BOSS2_UPDATE_SIG,
    BOSS2_HIT_SIG,      
    BOSS2_RESET_SIG
};

typedef struct {
    int16_t x;
    int16_t y;
    int16_t targetY; // <--  Để lưu tọa độ Y của xe tăng lúc bắn
    int8_t hp;
    bool is_active;
} boss2_cannon_bullet_t;

typedef struct {
    int16_t x;
    int16_t y;
    int8_t max_hp;
    int8_t hp;
    uint8_t explosion_timer;
    uint8_t fire_cooldown_counter;
    bool is_active;
    bool is_exploding;
    bool isDie;
    boss2_cannon_bullet_t cannon_bullet; 
} boss2_t;

extern boss2_t static_boss2;

#ifdef __cplusplus
extern "C" {
#endif
void check_and_spawn_boss2(uint32_t score);

void task_boss2_handle(struct ak_msg_t *msg);
void boss2_draw();

#ifdef __cplusplus
}
#endif

#endif // __BOSS2_OBJECT_H__
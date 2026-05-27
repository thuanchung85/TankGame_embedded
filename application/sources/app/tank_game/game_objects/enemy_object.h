#ifndef __ENEMY_OBJECT_H__
#define __ENEMY_OBJECT_H__

#include <stdint.h>
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
    ENEMY_SETUP_SIG = 1,
    ENEMY_UPDATE_SIG,
    ENEMY_RESET_SIG
};

typedef struct {
    bool isExploding;    
    int8_t enemy_type;
    int8_t hp;              
    int8_t explosionTimer;  
    short x;
} enemy_t;

extern enemy_t static_enemy;

#ifdef __cplusplus
extern "C" {
#endif
void check_collision_cannon_bullet_with_enemy();
void check_collision_minigun_bullet_with_enemy();
void check_collision_tank_with_enemy();

void task_enemy_handle(struct ak_msg_t *msg);
void enemy_draw();

#ifdef __cplusplus
}
#endif

#endif // __ENEMY_OBJECT_H__
#ifndef __MINIGUN_BULLET_OBJECT_H__
#define __MINIGUN_BULLET_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"
#include "tank_object.h"
#include "enemy_object.h"
#include "boss_object.h"
#include "boss2_object.h"
#include "trap_object.h"

#define MAX_MINIGUN_BULLETS  5  

struct ak_msg_t;

enum {
    MINIGUN_BULLET_SETUP_SIG = 1,
    MINIGUN_BULLET_UPDATE_SIG,
    MINIGUN_BULLET_FIRE_SIG,    
    MINIGUN_BULLET_RESET_SIG
};

typedef struct {
    int16_t x;
    int16_t y;
    int16_t targetY;
    bool is_active; 
} minigun_bullet_t;

extern minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];

#ifdef __cplusplus
extern "C" {
#endif

void task_minigun_bullet_handle(struct ak_msg_t *msg);
void minigun_bullet_draw();

#ifdef __cplusplus
}
#endif

#endif // __MINIGUN_BULLET_OBJECT_H__
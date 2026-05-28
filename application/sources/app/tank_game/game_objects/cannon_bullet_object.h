#ifndef __CANNON_BULLET_OBJECT_H__
#define __CANNON_BULLET_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"
#include "tank_object.h"

struct ak_msg_t;

enum {
    CANNON_BULLET_SETUP_SIG = 1,
    CANNON_BULLET_UPDATE_SIG,
    CANNON_BULLET_FIRE_SIG,    
    CANNON_BULLET_RESET_SIG
};

typedef struct {
    int16_t x, y;
    bool is_active; 
} cannon_bullet_t;

extern cannon_bullet_t static_cannon_bullet;

#ifdef __cplusplus
extern "C" {
#endif

void task_cannon_bullet_handle(struct ak_msg_t *msg);
void cannon_bullet_draw();

#ifdef __cplusplus
}
#endif

#endif // __CANNON_BULLET_OBJECT_H__
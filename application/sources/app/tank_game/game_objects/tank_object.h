#ifndef __TANK_OBJECT_H__
#define __TANK_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"
#include "task_display.h"
#include "game_screens/scr_game_over.h"

struct ak_msg_t;

enum {
    TANK_SETUP_SIG = 1,
    TANK_UPDATE_SIG,
    TANK_MOVE_FORWARD_SIG,  
    TANK_MOVE_BACKWARD_SIG, 
    TANK_RESET_SIG
};

typedef struct {
    int8_t x; //x of tank
    int8_t target_x; //x target for moving of tank
    int8_t frame_count;
    int8_t explosion_timer;
    bool is_cannon_reloaded;
    bool isExploding;
    bool isMoving;
    bool is_firing_gun;
    bool isDie;
} tank_t;

extern tank_t static_tank;

#ifdef __cplusplus
extern "C" {
#endif

void task_tank_handle(struct ak_msg_t *msg);
void tank_draw();

#ifdef __cplusplus
}
#endif

#endif // __TANK_OBJECT_H__
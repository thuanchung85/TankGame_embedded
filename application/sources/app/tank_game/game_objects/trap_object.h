#ifndef __TRAP_OBJECT_H__
#define __TRAP_OBJECT_H__

#include <stdint.h>
#include <stdlib.h>     
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"
#include "tank_object.h"
#include "minigun_bullet_object.h"

struct ak_msg_t;

enum {
    TRAP_SETUP_SIG = 1,  
    TRAP_UPDATE_SIG,     
    TRAP_RESET_SIG      
};

typedef struct {
    short x,y;
    bool is_active;
    bool isExploding;  
    bool isDestroy;
    int8_t explosionTimer;    

} trap_t;

extern trap_t static_trap;

#ifdef __cplusplus
extern "C" {
#endif

void task_trap_handle(struct ak_msg_t *msg);

void trap_draw();

#ifdef __cplusplus
}
#endif

#endif // __TRAP_OBJECT_H__
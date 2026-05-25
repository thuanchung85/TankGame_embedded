#ifndef __MOUNTAIN_OBJECT_H__
#define __MOUNTAIN_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"

struct ak_msg_t;

enum {
    MOUNTAIN_SETUP_SIG = 1,  
    MOUNTAIN_UPDATE_SIG,     
    MOUNTAIN_RESET_SIG      
};

typedef struct {
    short x;  
    int8_t frame_count;
} mountain_t;

extern mountain_t static_mountain;

#ifdef __cplusplus
extern "C" {
#endif

void task_mountain_handle(struct ak_msg_t *msg);

void mountain_draw();

#ifdef __cplusplus
}
#endif

#endif // __MOUNTAIN_OBJECT_H__
#ifndef __BUILDING_OBJECT_H__
#define __BUILDING_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"

struct ak_msg_t;

enum {
    BUILDING_SETUP_SIG = 1,  
    BUILDING_UPDATE_SIG,     
    BUILDING_RESET_SIG      
};

typedef struct {
    short x;  
    int8_t building_type; 
    int8_t frame_count;
} building_t;

extern building_t static_building;

#ifdef __cplusplus
extern "C" {
#endif

void task_building_handle(struct ak_msg_t *msg);

void building_draw();

#ifdef __cplusplus
}
#endif

#endif // __BUILDING_OBJECT_H__
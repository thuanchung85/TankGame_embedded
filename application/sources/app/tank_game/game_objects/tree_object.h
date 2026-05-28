#ifndef __TREE_OBJECT_H__
#define __TREE_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"

struct ak_msg_t;

enum {
    TREE_SETUP_SIG = 1,  
    TREE_UPDATE_SIG,     
    TREE_RESET_SIG      
};

typedef struct {
    short x;  
    int8_t tree_type; 
} tree_t;

extern tree_t static_tree;

#ifdef __cplusplus
extern "C" {
#endif

void task_tree_handle(struct ak_msg_t *msg);

void tree_draw();

#ifdef __cplusplus
}
#endif

#endif // __TREE_OBJECT_H__
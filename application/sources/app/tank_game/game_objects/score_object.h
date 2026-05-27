#ifndef __SCORE_OBJECT_H__
#define __SCORE_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "app_dbg.h"
#include "message.h"
#include "enemy_object.h"

struct ak_msg_t;

enum {
    SCORE_SETUP_SIG = 1,
    SCORE_ADD_SIG,       
    SCORE_RESET_SIG     
};

typedef struct {
    uint32_t current_score; 
} score_object_t;

extern score_object_t static_score;

#ifdef __cplusplus
extern "C" {
#endif
void add_score();

void task_score_handle(struct ak_msg_t *msg);

void score_draw();

#ifdef __cplusplus
}
#endif

#endif // __SCORE_OBJECT_H__
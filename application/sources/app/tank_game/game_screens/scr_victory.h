#ifndef __SCR_VICTORY_H__
#define __SCR_VICTORY_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "score_object.h"

extern view_dynamic_t dyn_view_victory;
extern view_screen_t scr_victory;
extern void scr_victory_handle(ak_msg_t* msg);

#endif 
#ifndef __SCR_GAME_OVER_H__
#define __SCR_GAME_OVER_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "score_object.h"

extern view_dynamic_t dyn_view_game_over;
extern view_screen_t scr_game_over;
extern void scr_game_over_handle(ak_msg_t* msg);

#endif 
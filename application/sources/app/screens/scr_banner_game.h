#ifndef __SCR_BANNER_GAME_H__
#define __SCR_BANNER_GAME_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"


extern view_dynamic_t dyn_view_banner_game;
extern view_screen_t scr_banner_game;
extern void scr_banner_game_handle(ak_msg_t* msg);

#endif 
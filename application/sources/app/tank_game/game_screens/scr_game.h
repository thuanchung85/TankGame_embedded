#ifndef __SCR_GAME_H__
#define __SCR_GAME_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "../../../../driver/eeprom/eeprom.h" //save score to eeprom

extern view_dynamic_t dyn_view_game;
extern view_screen_t scr_game;
extern void scr_game_handle(ak_msg_t* msg);

#endif 
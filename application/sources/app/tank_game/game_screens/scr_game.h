#ifndef __SCR_GAME_H__
#define __SCR_GAME_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "../../../../driver/eeprom/eeprom.h" //save score to eeprom
#include "game_objects/ground_object.h"// include mặt đất
#include "game_objects/tree_object.h" //  include cây
#include "game_objects/building_object.h" //  include nhà
#include "game_objects/mountain_object.h" //  include núi

extern view_dynamic_t dyn_view_game;
extern view_screen_t scr_game;
extern void scr_game_handle(ak_msg_t* msg);

#endif 
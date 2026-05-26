#ifndef __SCR_GAME_H__
#define __SCR_GAME_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "../../../../driver/eeprom/eeprom.h" //save score to eeprom
#include "game_objects/ground_object.h"// ground_object
#include "game_objects/tree_object.h" //  tree
#include "game_objects/building_object.h" //  building
#include "game_objects/mountain_object.h" //  mountain
#include "game_objects/tank_object.h" //  include tank
#include "game_objects/cannon_bullet_object.h" //  include cannon bullet
#include "game_objects/enemy_object.h" //  include enemy
#include "game_objects/minigun_bullet_object.h" //  include minigun
#include "game_objects/score_object.h" //score
#include "game_objects/boss_object.h" //boss

extern view_dynamic_t dyn_view_game;
extern view_screen_t scr_game;
extern void scr_game_handle(ak_msg_t* msg);

#endif 
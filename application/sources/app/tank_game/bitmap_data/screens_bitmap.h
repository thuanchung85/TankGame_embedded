#ifndef __SCREENS_BITMAP_H__
#define __SCREENS_BITMAP_H__

#include "view_render.h"
extern  bool check_collision(int16_t x1, int16_t y1, int16_t w1, int16_t h1,
                            int16_t x2, int16_t y2, int16_t w2, int16_t h2);


extern const unsigned char PROGMEM bitmap_tank [];
extern const unsigned char PROGMEM bitmap_ground [];
extern const unsigned char PROGMEM bitmap_coconutTree [];
extern const unsigned char PROGMEM bitmap_dryTree [];
extern const unsigned char PROGMEM bitmap_grassesTree [];
extern const unsigned char PROGMEM bitmap_theMountain [];
extern const unsigned char PROGMEM bitmap_house1 [];
extern const unsigned char PROGMEM bitmap_house2 [];
extern const unsigned char PROGMEM bitmap_house3 [];
extern const unsigned char PROGMEM bitmap_house4 [];
extern const unsigned char PROGMEM bitmap_myTank_cannon_bullet [];
extern const unsigned char PROGMEM bitmap_HP [];
extern const unsigned char PROGMEM bitmap_myTank_gun_bullet [];
extern const unsigned char PROGMEM bitmap_enemy_tank [];
extern const unsigned char PROGMEM bitmap_enemy_air_plane [];
extern const unsigned char PROGMEM bitmap_enemy_mine [];
extern const unsigned char PROGMEM bitmap_enemy_rocket [];
extern const unsigned char PROGMEM bitmap_enemy_troop [];
extern const unsigned char PROGMEM bitmap_bum[];
extern const unsigned char PROGMEM bitmap_bum2[];
extern const unsigned char PROGMEM bitmap_bum3[];
extern const unsigned char PROGMEM bitmap_game_pause[];
extern const unsigned char PROGMEM bitmap_banner_game[];
extern const unsigned char PROGMEM bitmap_game_over[];
extern const unsigned char PROGMEM bitmap_boss[];
extern const unsigned char PROGMEM bitmap_boss2[];
extern const unsigned char PROGMEM bitmap_boss_rocket[];
extern const unsigned char PROGMEM bitmap_boss2_cannon_bullet[];
extern const unsigned char PROGMEM bitmap_victory[];

#endif

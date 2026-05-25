#ifndef __GAME_EVENTS_MANAGER_H__
#define __GAME_EVENTS_MANAGER_H__

#include <stdint.h>
#include "../game_objects/Boss.h"
#include "../game_objects/Tank.h"
#include "../game_objects/Score.h"
#include "../../../../driver/eeprom/eeprom.h" //save score to eeprom

class GameEventsManager {
public:
    bool isPaused;
    bool isGameOver;
    bool isVictory;
    uint8_t game_over_delay_counter;

    GameEventsManager();

    void check_boss_spawn(Score& score, Boss& boss);
    void check_victory_condition(Boss& boss, Score& score);
    void check_game_over_condition(Tank& tank, Score& score);
    
    void reset_all_game_objects(Tank& tank, Score& score, Boss& boss); 
    void update_top_scores(uint32_t new_score);
};

#endif // __GAME_EVENTS_MANAGER_H__
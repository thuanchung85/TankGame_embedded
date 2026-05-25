#include "GameEventsManager.h"
#include "app_dbg.h" // debug APP_DBG


extern void reset_other_objects(); 

GameEventsManager::GameEventsManager() {
    isPaused = false;
    isGameOver = false;
    isVictory = false;
    game_over_delay_counter = 0;
}

void GameEventsManager::check_boss_spawn(Score& score, Boss& boss) {
    if (score.current_score >= 200 && !boss.is_active && boss.hp == boss.max_hp && !boss.is_exploding) {
        boss.spawn();
    }
}

void GameEventsManager::check_victory_condition(Boss& boss, Score& score) {
    if (boss.isDie && !isVictory) {
        isVictory = true; //  Victory 
        update_top_scores(score.current_score); 
        APP_DBG("GAME VICTORY! Boss is Dead.\n");
        BUZZER_PlaySound(BUZZER_SOUND_MERRY_CHRISTMAS);
    }
}

void GameEventsManager::check_game_over_condition(Tank& tank, Score& score) {
    if (tank.isExploding) {
        game_over_delay_counter++;
        
        if (game_over_delay_counter == 1) {
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
        }

        if (game_over_delay_counter >= 30) {
            isGameOver = true; // Game Over 
            update_top_scores(score.current_score);
        }
    }
}

void GameEventsManager::update_top_scores(uint32_t new_score) {
    uint32_t top[3];
    eeprom_read(0x00, (uint8_t*)&top[0], 4);
    eeprom_read(0x04, (uint8_t*)&top[1], 4);
    eeprom_read(0x08, (uint8_t*)&top[2], 4);

    if (new_score > top[0]) {
        top[2] = top[1]; top[1] = top[0]; top[0] = new_score;
    } else if (new_score > top[1]) {
        top[2] = top[1]; top[1] = new_score;
    } else if (new_score > top[2]) {
        top[2] = new_score;
    } else {
        return;
    }

    eeprom_write(0x00, (uint8_t*)&top[0], 4);
    eeprom_write(0x04, (uint8_t*)&top[1], 4);
    eeprom_write(0x08, (uint8_t*)&top[2], 4);
    APP_DBG("EEPROM: New High Score Saved!\n");
}
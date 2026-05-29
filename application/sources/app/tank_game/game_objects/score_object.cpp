#include "score_object.h"

//============  UPDATE TOP SCORE  =====//
void update_top_scores(uint32_t new_score) {
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


score_object_t static_score = {
    .current_score = 0
};

// function add score
 void add_score()
{
    uint8_t score_bonus = 1;

    if (static_enemy.enemy_type == 1)
    {
        // airplane: random  5 - 20 score
        score_bonus = 5 + (rand() % 16);
    }
    else
    {
        // other enemy random  1 - 10 score
        score_bonus = 1 + (rand() % 10);
    }

    task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &score_bonus, sizeof(uint8_t));
}


void task_score_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case SCORE_SETUP_SIG:
        case SCORE_RESET_SIG: {
            static_score.current_score = 0;
        } break;

        case SCORE_ADD_SIG: {
            uint8_t* msg_data = get_data_common_msg(msg); 
            
            uint8_t points_to_add = msg_data[0]; 
            
            static_score.current_score += points_to_add;
            
            APP_DBG(">> Score Updated: +%d | Total: %ld\n", points_to_add, static_score.current_score);
        } break;

        default:
            break;
    }
}

void score_draw() {
    view_render.setCursor(105, 1); 
    view_render.setTextSize(1);     
    view_render.setTextColor(WHITE); 
    
    char scoreStr[7];
    sprintf(scoreStr, "%ld", static_score.current_score);
    
    for(int i = 0; scoreStr[i] != '\0'; i++) {
        view_render.write(scoreStr[i]); 
    }
}
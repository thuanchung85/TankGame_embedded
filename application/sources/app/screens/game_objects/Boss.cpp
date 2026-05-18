#include "Boss.h"

Boss::Boss() {
    reset();
}

void Boss::reset() {
    x = 130;        
    y = 14;         
    max_hp = 10;    
    hp = max_hp;
    is_active = false;
    is_exploding = false;
    explosion_timer = 0;
    move_dir = 1;
    isDie = false;
    rocket.reset(2);
    fire_cooldown_counter = 0;
}

void Boss::spawn() {
    if (!is_active && !is_exploding && !isDie) {
        reset();
        is_active = true;
    }
}

void Boss::lose_hp(int8_t damage) {
    if (!is_active) return;
    hp -= damage;
    if (hp <= 0) {
        is_active = false;
        is_exploding = true;
        explosion_timer = 0;
    }
}

void Boss::update() {
    if (is_exploding) {
        explosion_timer++;
        if (explosion_timer > 15) { 
            is_exploding = false;
            isDie = true; 
        }
        return;
    }

    if (isDie || !is_active) return; 

    if (x > 65) {
        x -= 1; 
    } else {
        y += move_dir;
        if (y <= 2 || y >= 26) { 
            move_dir = -move_dir; 
        }
    }

    if (!rocket.is_active) {
        fire_cooldown_counter++;
        if (fire_cooldown_counter >= 68) { 
            if (rand() % 2 == 0) { 
                rocket.is_active = true;
                rocket.x = x + 20; 
                rocket.y = y + 12;
                rocket.hp = 2;
                BUZZER_PlaySound(BUZZER_SOUND_FIRECRACKER);
            }
            fire_cooldown_counter = 0;
            
        }
    }

    if (rocket.is_active) {
        rocket.x -= 2; 
        if (rocket.x < -17) { 
            rocket.is_active = false;
        }
    }
}

void Boss::draw() {

    //boss rocket
    if (rocket.is_active) {
        view_render.drawBitmap(rocket.x, rocket.y, bitmap_boss_rocket, 17, 11, WHITE);
    }
    //boss
    if (is_active) {
        view_render.drawBitmap(x, y, bitmap_boss, 60, 36, WHITE);
        view_render.setTextSize(1);
        view_render.setCursor(20, 0); 
        view_render.print("BOSS"); 
        view_render.drawRect(52, 2, 50, 3, WHITE); 
        
        //boss hp bar
        int8_t hp_bar_width = (hp * 50) / max_hp;
        view_render.fillRect(52, 2, hp_bar_width, 3, WHITE);

    } 
    else if (is_exploding) 
    {
        view_render.drawCircle(x + 30, y + 18, explosion_timer * 2, WHITE);
        // animation explosion boss
        if (explosion_timer < 3) {
            view_render.drawBitmap(x + 30, y + 18, bitmap_bum, 28, 20, WHITE);
        } else if (explosion_timer < 6) {
            view_render.drawBitmap(x + 30, y + 18, bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(x + 30, y + 18, bitmap_bum3, 30, 31, WHITE);
        }
    }
}

bool Boss::check_collision(int16_t bx, int16_t by, int8_t bw, int8_t bh) {
    if (!is_active) return false;
    return (bx < x + 60 && bx + bw > x && by < y + 36 && by + bh > y);
}
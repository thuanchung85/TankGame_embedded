#include "boss2_object.h"

boss2_t static_boss2;

//=========BOSS2=======//
// cannon hit BOSS2
static void check_collision_cannon_bullets_with_boss2()
{
    if (!static_boss2.is_active || static_boss2.is_exploding || static_boss2.isDie)
        return;

    // --- Check tank cannon hit Boss2 ---
    if (static_cannon_bullet.is_active)
    {
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_boss2.x, static_boss2.y, 42, 34))
        {
            static_cannon_bullet.is_active = false; 

            uint8_t dmg = 1;
            task_post_common_msg(TG_BOSS2_TASK_ID, BOSS2_HIT_SIG, &dmg, sizeof(uint8_t));
        }
    }
}

// tank collision with Boss2
static void check_collision_tank_with_boss2()
{
    if (!static_boss2.is_active || static_tank.isExploding)
        return;

    // case 1:  boss 2 fire cannon bullet hit tank
    if (static_boss2.cannon_bullet.is_active)
    {
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_boss2.cannon_bullet.x, static_boss2.cannon_bullet.y, 13, 5))
        {
            static_boss2.cannon_bullet.is_active = false; 
            static_tank.isExploding = true;               
            static_tank.explosion_timer = 0;
            // APP_DBG(">> TANK DESTROYED BY BOSS2 CANNON! <<\n");
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
        }
    }

    // case 2:  Boss2 collision with tank
    if (check_collision(static_tank.x, 30, 30, 30,
                        static_boss2.x, static_boss2.y, 42, 34))
    {
        static_tank.isExploding = true;
        static_tank.explosion_timer = 0;
        // APP_DBG(">> TANK CRASHED BY BOSS2! <<\n");
        BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
    }
}

// Boss2 Spawn when Score
 void check_and_spawn_boss2(uint32_t score)
{
    extern score_object_t static_score; 

    //call boss when score 
    if (static_score.current_score >= score && !static_boss2.is_active && !static_boss2.isDie && !static_boss2.is_exploding)
    {
        APP_DBG(">> BOSS2_SPAWN_SIG!\n");
        task_post_pure_msg(TG_BOSS2_TASK_ID, BOSS2_SPAWN_SIG);
    }
}

//  Minigun hit cannon_bullet of Boss2
static void check_collision_minigun_with_boss2_cannon_bullet()
{
    if (!static_boss2.is_active || !static_boss2.cannon_bullet.is_active)
        return;

    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++)
    {
        if (minigun_pool[i].is_active)
        {
            if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                static_boss2.cannon_bullet.x, static_boss2.cannon_bullet.y, 13, 5))
            {
                minigun_pool[i].is_active = false;
                if (static_boss2.cannon_bullet.hp > 0)
                {
                    static_boss2.cannon_bullet.hp--;
                }

                if (static_boss2.cannon_bullet.hp == 0)
                {
                    static_boss2.cannon_bullet.is_active = false;
                    // APP_DBG(">> PLAYER DESTROYED BOSS ROCKET! <<\n");
                    BUZZER_PlaySound(BUZZER_SOUND_BANG);
                }

                break; // exit loop
            }
        }
    }
}

//boss2 reset data
static void boss2_reset()
{
    static_boss2.x = 130;
    static_boss2.y = 14;
    static_boss2.max_hp = 30;
    static_boss2.hp = static_boss2.max_hp;
    static_boss2.is_active = false;
    static_boss2.is_exploding = false;
    static_boss2.explosion_timer = 0;
    static_boss2.isDie = false;
    static_boss2.fire_cooldown_counter = 0;

    static_boss2.cannon_bullet.is_active = false;
    static_boss2.cannon_bullet.x = 0;
    static_boss2.cannon_bullet.y = 0;
    static_boss2.cannon_bullet.hp = 1;
}

//boss 2 moving
static void boss2_moving()
{
    static int8_t boss2_dir_x = -1; // -1 will move left, 1 will move right

    // update X for boss2
    static_boss2.x += boss2_dir_x;

    // bounding
    if (static_boss2.x <= 60)
    {
        boss2_dir_x = 1; // hit left (X=60) -> then move right
    }
    else if (static_boss2.x >= 120)
    {
        boss2_dir_x = -1; // hit right (X=120) -> then move left
    }
}

//boss 2 fire cannon
static void boss2_fire_cannon(){
    //fire cannon
    if (!static_boss2.cannon_bullet.is_active)
    {
        static_boss2.fire_cooldown_counter++;
        if (static_boss2.fire_cooldown_counter >= 68)
        {
            if (rand() % 2 == 0)
            {
                static_boss2.cannon_bullet.is_active = true;
                static_boss2.cannon_bullet.x = static_boss2.x;
                static_boss2.cannon_bullet.y = static_boss2.y + 12; 

                static_boss2.cannon_bullet.targetY = 40;//target to y 40

                static_boss2.cannon_bullet.hp = 1;
                APP_DBG(">> BOSS 2 FIRED CANNON AT TANK! <<\n");
                 BUZZER_PlaySound(BUZZER_SOUND_FIRECRACKER);
            }
            static_boss2.fire_cooldown_counter = 0;
        }
    }
}


//============  HANDLE  ==========//
void task_boss2_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {
        //when boss init data
        case BOSS2_SETUP_SIG:
        case BOSS2_RESET_SIG:
        {
            boss2_reset();
        }
        break;

        //when boss spawn
        case BOSS2_SPAWN_SIG:
        {
            if (!static_boss2.is_active && !static_boss2.is_exploding && !static_boss2.isDie)
            {
                boss2_reset();
                static_boss2.is_active = true;
                APP_DBG(">> BOSS 2 WARNING: SPAWNING! <<\n");
            }
        }
        break;

        //when boss hit
        case BOSS2_HIT_SIG:
        {
            if (!static_boss2.is_active)
                break;

            uint8_t damage = 1;
            uint8_t *msg_data = get_data_common_msg(msg);
            if (msg_data != nullptr && msg_data != NULL)
            {
                damage = msg_data[0];
            }

            static_boss2.hp -= damage;
            APP_DBG(">> BOSS 2 HIT! HP: %d/%d\n", static_boss2.hp, static_boss2.max_hp);

            if (static_boss2.hp <= 0)
            {
                static_boss2.is_active = false;
                static_boss2.is_exploding = true;
                static_boss2.explosion_timer = 0;
                APP_DBG(">> BOSS 2 DESTROYED! <<\n");
                BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                uint8_t boss2_score_bonus = 100;
                task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &boss2_score_bonus, sizeof(uint8_t));
            }
        }
        break;

        //when boss update loop
        case BOSS2_UPDATE_SIG:
        {
            // =========================================================
            // ACTIONS OF BOSS 
            // =========================================================
            if (!static_boss2.isDie && static_boss2.is_active && !static_boss2.is_exploding)
            {
                boss2_moving();
                boss2_fire_cannon();
            }
            // --- update cannon bullet ---
            if (static_boss2.cannon_bullet.is_active)
            {
                static_boss2.cannon_bullet.x -= 1; 
                if (static_boss2.cannon_bullet.y < static_boss2.cannon_bullet.targetY)
                {
                    static_boss2.cannon_bullet.y += 1; 
                }
                else if (static_boss2.cannon_bullet.y > static_boss2.cannon_bullet.targetY)
                {
                    static_boss2.cannon_bullet.y -= 1; 
                }

                // move out screen
                if (static_boss2.cannon_bullet.x < -17 || static_boss2.cannon_bullet.y > 64 || static_boss2.cannon_bullet.y < -5)
                {
                    static_boss2.cannon_bullet.is_active = false;
                }
            }
            
            // =========================================================
            // COLLISION OF BOSS
            // =========================================================
            if (static_boss2.is_active && !static_boss2.isDie)
            {
                check_collision_cannon_bullets_with_boss2();
                check_collision_minigun_with_boss2_cannon_bullet();
                check_collision_tank_with_boss2();
            }

            // --- update Boss explosion animation ---//
            if (static_boss2.is_exploding)
            {
                static_boss2.explosion_timer++;
                if (static_boss2.explosion_timer > 15)
                {
                    static_boss2.is_exploding = false;
                    static_boss2.isDie = true;
                }
                break;
            }

        }
        break;

        default:
            break;
    }
}

//===============  DRAW  ==============//
void boss2_draw()
{
    //draw boss2 cannon bullet
    if (static_boss2.cannon_bullet.is_active && !static_boss2.isDie) 
    {
        view_render.drawBitmap(static_boss2.cannon_bullet.x, static_boss2.cannon_bullet.y, bitmap_boss2_cannon_bullet, 13, 5, WHITE);
    }

    //draw boss2
    if (static_boss2.is_active)
    {
        view_render.drawBitmap(static_boss2.x, static_boss2.y, bitmap_boss2, 42, 34, WHITE);

        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(20, 0);
        view_render.print("BOSS");
        view_render.drawRect(52, 2, 50, 3, WHITE);

        if (static_boss2.hp > 0)
        {
            int8_t hp_bar_width = (static_boss2.hp * 50) / static_boss2.max_hp;
            view_render.fillRect(52, 2, hp_bar_width, 3, WHITE);
        }
    }

    // draw boss2 explosion animation
    else if (static_boss2.is_exploding)
    {
        view_render.drawCircle(static_boss2.x + 30, static_boss2.y + 18, static_boss2.explosion_timer * 2, WHITE);

        if (static_boss2.explosion_timer < 3)
        {
            view_render.drawBitmap(static_boss2.x + 30, static_boss2.y + 18, bitmap_bum, 28, 20, WHITE);
        }
        else if (static_boss2.explosion_timer < 6)
        {
            view_render.drawBitmap(static_boss2.x + 30, static_boss2.y + 18, bitmap_bum2, 30, 26, WHITE);
        }
        else
        {
            view_render.drawBitmap(static_boss2.x + 30, static_boss2.y + 18, bitmap_bum3, 30, 31, WHITE);
        }

        if (static_boss2.explosion_timer < 3)
        {
            view_render.drawBitmap(static_boss2.x, static_boss2.y, bitmap_bum, 28, 20, WHITE);
        }
        else if (static_boss2.explosion_timer < 6)
        {
            view_render.drawBitmap(static_boss2.x, static_boss2.y, bitmap_bum2, 30, 26, WHITE);
        }
        else
        {
            view_render.drawBitmap(static_boss2.x, static_boss2.y, bitmap_bum3, 30, 31, WHITE);
        }
    }
}
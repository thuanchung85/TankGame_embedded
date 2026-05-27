#include "boss_object.h"

boss_t static_boss;

//======== FUNCTIONS =========//
// cannon hit BOSS
static void check_collision_cannon_bullets_with_boss()
{
    if (!static_boss.is_active || static_boss.is_exploding || static_boss.isDie)
        return;

    // --- Check hit Boss ---
    if (static_cannon_bullet.is_active)
    {
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_boss.x, static_boss.y, 60, 36))
        {
            static_cannon_bullet.is_active = false; // remove cannon bullet

            // task Boss hit
            uint8_t dmg = 1;
            task_post_common_msg(TG_BOSS_TASK_ID, BOSS_HIT_SIG, &dmg, sizeof(uint8_t));
        }
    }
}

// tank collision with Boss and rocket boss
static void check_collision_tank_with_boss()
{
   if (static_tank.isExploding) return;

    // --- case 1: Rocket's Boss hit tank ---
    if (static_boss.rocket.is_active)
    {
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_boss.rocket.x, static_boss.rocket.y, 15, 8))
        {
            static_boss.rocket.is_active = false; // Rocket bum
            static_tank.isExploding = true;       // tank bum
            static_tank.explosion_timer = 0;
            // APP_DBG(">> TANK DESTROYED BY BOSS ROCKET! <<\n");
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
        }
    }

    // --- case 2: Boss direct collision hit tank ---
    if (static_boss.is_active && !static_boss.is_exploding && !static_boss.isDie)
    {
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_boss.x, static_boss.y, 60, 36))
        {
            static_tank.isExploding = true;
            static_tank.explosion_timer = 0;
            // APP_DBG(">> TANK CRASHED BY BOSS! <<\n");
            BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
        }
    }
}

// Boss Spawn when Score
 void check_and_spawn_boss(uint8_t score)
{
    extern score_object_t static_score; 
    // If the score reaches score points or more AND the Boss has never appeared/is not dead, then summon the Boss!
    if (static_score.current_score >= score && !static_boss.is_active && !static_boss.isDie && !static_boss.is_exploding)
    {
        APP_DBG(">> BOSS_SPAWN_SIG!\n");
        task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_SPAWN_SIG);
    }
}

//  Minigun hit Rocket of Boss
static void check_collision_minigun_with_boss_rocket()
{
    if (!static_boss.rocket.is_active)
        return;

    // Browse through my entire pool of flying Minigun ammunition
    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++)
    {
        if (minigun_pool[i].is_active)
        {
            // Collision Check: Minigun rounds (2x1) hit Boss Rocket (17x11)
            if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                static_boss.rocket.x, static_boss.rocket.y, 17, 11))
            {
                minigun_pool[i].is_active = false; //Our Minigun bullet has disappeared.

                //Reduce the health of the Rocket Boss.
                if (static_boss.rocket.hp > 0)
                {
                    static_boss.rocket.hp--;
                }

                // If Rocket runs out of health, it will detonate/disappear immediately in mid-air.
                if (static_boss.rocket.hp == 0)
                {
                    static_boss.rocket.is_active = false;
                    // APP_DBG(">> PLAYER DESTROYED BOSS ROCKET! <<\n");
                    BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); // Phát âm thanh đúng 1 lần duy nhất tại đây!
                }

                break; // Exit this frame loop to process the next bullet
            }
        }
    }
}

//reset boss
static void boss_reset()
{
    static_boss.x = 130;
    static_boss.y = 14;
    static_boss.max_hp = 20;
    static_boss.hp = static_boss.max_hp;
    static_boss.is_active = false;
    static_boss.is_exploding = false;
    static_boss.explosion_timer = 0;
    static_boss.move_dir = 1;
    static_boss.move_dir_x = 1; 
    static_boss.isDie = false;
    static_boss.fire_cooldown_counter = 0;

    static_boss.rocket.is_active = false;
    static_boss.rocket.x = 0;
    static_boss.rocket.y = 0;
    static_boss.rocket.hp = 2;
}

//boss moving
static void boss_moving()
{
    // --- AI Boss Movement (Comprehensive Zig-zag Surfing) ---
    // 1. state 1: When it first appears at X=130, it slowly flies towards the screen until it reaches X=90
    if (static_boss.x > 90 && static_boss.move_dir_x == 1)
    {
        static_boss.x -= 1;
    }
    // 2. State 2: Once in battle, activate the zig-zag movement (back and forth + up and down).
    else
    {
        // --- edit Y (UP / DOWN) ---
        static_boss.y += static_boss.move_dir;
        if (static_boss.y <= 2 || static_boss.y >= 26)
        {
            static_boss.move_dir = -static_boss.move_dir; // If you hit the top/bottom boundary, change direction.
        }

        // --- edit X (forward / back) ---
        // move_dir_x = 1 means the Boss is moving to the right, move_dir_x = -1 means it is moving to the left
        static_boss.x += static_boss.move_dir_x;

        // Set flight range limits for the X-axis (From X=40 to X=90)
        if (static_boss.x <= 40)
        {
            static_boss.move_dir_x = 1; // Touch the left side -> Move to the right
        }
        else if (static_boss.x >= 90)
        {
            static_boss.move_dir_x = -1; // Right side contact -> Move to the left
        }
    }
}
//boss fire rocket
static void boss_fire_rocket()
{
    // --- Logic Cooldown , boss reload và shoot Rocket ---
    if (!static_boss.rocket.is_active)
    {
        static_boss.fire_cooldown_counter++;
        if (static_boss.fire_cooldown_counter >= 68)
        {
            if (rand() % 2 == 0)
            {
                static_boss.rocket.is_active = true;
                static_boss.rocket.x = static_boss.x; // The bullet flew to the left, so the launch was slightly to the left, Boss.
                static_boss.rocket.y = static_boss.y + 12;
                static_boss.rocket.hp = 2;
                BUZZER_PlaySound(BUZZER_SOUND_FIRECRACKER);
                APP_DBG(">> BOSS FIRED ROCKET! <<\n");
            }
            static_boss.fire_cooldown_counter = 0;
        }
    }
}



//============ HANDLE ==========//
void task_boss_handle(ak_msg_t *msg) {
    switch (msg->sig) {

        //when boss init data
        case BOSS_SETUP_SIG:
        case BOSS_RESET_SIG: {
          boss_reset();
        } break;

        //when boss spawn
        case BOSS_SPAWN_SIG: {
           if (!static_boss.is_active && !static_boss.is_exploding && !static_boss.isDie) {
                boss_reset();
                static_boss.is_active = true; //active boss
                APP_DBG(">> BOSS WARNING: SPAWNING! <<\n");
            }
        } break;

        //when boss hit by tank
        case BOSS_HIT_SIG: {
           if (!static_boss.is_active) break;

            uint8_t damage = 1; // default 1 hit 1 damage
            // Security check: Read data only if this message actually contains a payload
            uint8_t* msg_data = get_data_common_msg(msg); 
            if (msg_data != nullptr && msg_data != NULL) {
                damage = msg_data[0]; 
            }

            static_boss.hp -= damage;
            APP_DBG(">> BOSS HIT! HP: %d/%d\n", static_boss.hp, static_boss.max_hp);

            if (static_boss.hp <= 0) {
                static_boss.is_active = false;
                static_boss.is_exploding = true;
                static_boss.explosion_timer = 0;
                APP_DBG(">> BOSS DESTROYED! <<\n");
                BUZZER_PlaySound(BUZZER_SOUND_EXPLOSION); 
                //  REWARD 100 SCORES
                uint8_t boss_score_bonus = 100; 
                task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &boss_score_bonus, sizeof(uint8_t));
            }
        } break;

        //when boss update loop
        case BOSS_UPDATE_SIG: {
           
            // =========================================================
            // ACTIONS OF BOSS 
            // =========================================================
            if (!static_boss.isDie && static_boss.is_active && !static_boss.is_exploding)
            {
                boss_moving();
                boss_fire_rocket();
            }
            // --- update Rocket data ---//
            if (static_boss.rocket.is_active)
            {
                static_boss.rocket.x -= 1; // The rocket flew continuously to the left, towards the tank.
                if (static_boss.rocket.x < -17)
                {
                    static_boss.rocket.is_active = false; // If it flies off the screen, it turns off.
                }
            }

            // =========================================================
            // COLLISION OF BOSS 
            // =========================================================
            if (static_boss.is_active && !static_boss.isDie) {
                // 1. Automatically check for cannonball hits boss
                check_collision_cannon_bullets_with_boss();
                // 2. Self-check your Minigun's interception and destruction of boss Rocket.
                check_collision_minigun_with_boss_rocket();
                // 3. Check boss or the rocket will hit our tank.
               check_collision_tank_with_boss();
            }
            
            // --- update Boss explosion animation ---//
            if (static_boss.is_exploding)
            {
                static_boss.explosion_timer++;
                if (static_boss.explosion_timer > 15)
                {
                    static_boss.is_exploding = false;
                    static_boss.isDie = true;
                }
            }
        
        } break;

        default:
            break;
    }
}



//===============  DRAW ==============//
void boss_draw() {
    //  Rocket  Boss
    if (static_boss.rocket.is_active && !static_boss.isDie) {
        view_render.drawBitmap(static_boss.rocket.x, static_boss.rocket.y, bitmap_boss_rocket, 17, 11, WHITE);
    }

    // Boss , HP
    if (static_boss.is_active) {
        view_render.drawBitmap(static_boss.x, static_boss.y, bitmap_boss, 60, 36, WHITE);
        
        view_render.setTextSize(1);
        view_render.setTextColor(WHITE);
        view_render.setCursor(20, 0); 
        view_render.print("BOSS"); 
        view_render.drawRect(52, 2, 50, 3, WHITE); 
        
        if (static_boss.hp > 0) {
            int8_t hp_bar_width = (static_boss.hp * 50) / static_boss.max_hp;
            view_render.fillRect(52, 2, hp_bar_width, 3, WHITE);
        }
    } 
    // explosion animation
    else if (static_boss.is_exploding) {
        view_render.drawCircle(static_boss.x + 30, static_boss.y + 18, static_boss.explosion_timer * 2, WHITE);
        
        if (static_boss.explosion_timer < 3) {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum, 28, 20, WHITE);
        } else if (static_boss.explosion_timer < 6) {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(static_boss.x + 30, static_boss.y + 18, bitmap_bum3, 30, 31, WHITE);
        }

        if (static_boss.explosion_timer < 3) {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum, 28, 20, WHITE);
        } else if (static_boss.explosion_timer < 6) {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(static_boss.x , static_boss.y , bitmap_bum3, 30, 31, WHITE);
        }
    }
}
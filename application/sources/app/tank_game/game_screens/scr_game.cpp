#include "scr_game.h"

//  ================  VARIABLES  ================ //
static bool is_minigun_firing = false; //for active minigun fire

// variable access tank, cannon bullet...enemies
extern tank_t static_tank;
extern cannon_bullet_t static_cannon_bullet;
extern enemy_t static_enemy;
extern boss_t static_boss;
extern score_object_t static_score; 
extern minigun_bullet_t minigun_pool[MAX_MINIGUN_BULLETS];

//  ================  HELPER FUNCTIONS ================ //
//function add score
static void add_score()
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

    task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &score_bonus , sizeof(uint8_t));
}

// check if game objects rectangle over each other
static bool check_collision(int16_t x1, int16_t y1, int16_t w1, int16_t h1,
                            int16_t x2, int16_t y2, int16_t w2, int16_t h2) 
{
    return (x1 < x2 + w2 &&
            x1 + w1 > x2 &&
            y1 < y2 + h2 &&
            y1 + h1 > y2);
}

// function fire minigun
static void minigun_fire()
{
    if (is_minigun_firing)
    {
        static uint8_t minigun_fire_divider = 0; // bullets delay of minigun
        minigun_fire_divider++;
        if (minigun_fire_divider >= 8)
        { // delay 8 loop (120ms)
            task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_FIRE_SIG);
            minigun_fire_divider = 0;
        }
    }
}

//function check collision cannon bullet with enemy
static void check_collision_cannon_bullet_with_enemy()
{
    if (static_cannon_bullet.is_active && !static_enemy.isExploding)
    {
        // get (Bounding Box) of enemy type
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;

        if (static_enemy.enemy_type == 1)
            enemy_y = 5; // for air plane
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        } // for troop

        // Check cannon bullet (5x3) hit test enemy
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_enemy.x, enemy_y, enemy_w, enemy_h))
        {
            // deactive cannon bullet
            static_cannon_bullet.is_active = false;

            // enemy loss hp
            if (static_enemy.hp > 0)
                static_enemy.hp--;

            // if no hp then enemy destroy
            if (static_enemy.hp == 0)
            {
                static_enemy.isExploding = true;
                static_enemy.explosionTimer = 0;
                // APP_DBG(">> ENEMY DESTROYED!\n");
                add_score();
            }
            else
            {
                // APP_DBG(">> ENEMY HIT! HP REMAINING: %d\n", static_enemy.hp);
            }
        }
    }
}

//function check collision minigun bullet with enemy
static void check_collision_minigun_bullet_with_enemy()
{
    if (!static_enemy.isExploding)
    {
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
        if (static_enemy.enemy_type == 1)
            enemy_y = 5;
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        }

        // loop for array of minigun bullet
        for (int i = 0; i < MAX_MINIGUN_BULLETS; i++)
        {
            if (minigun_pool[i].is_active)
            {
                if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                    static_enemy.x, enemy_y, enemy_w, enemy_h))
                {
                    minigun_pool[i].is_active = false; // deactive the bullet hit enemy

                    if (static_enemy.hp > 0)
                        static_enemy.hp--;

                    if (static_enemy.hp == 0)
                    {
                        static_enemy.isExploding = true;
                        static_enemy.explosionTimer = 0;

                        //add score
                       add_score();
                    }
                    break; 
                }
            }
        }
    }
}

//function check collision tank with enemy
static void check_collision_tank_with_enemy()
{
    if (!static_tank.isExploding && !static_enemy.isExploding)
    {
        int16_t enemy_w = 25, enemy_h = 21, enemy_y = 33;
        if (static_enemy.enemy_type == 1)
            enemy_y = 5;
        else if (static_enemy.enemy_type == 3)
        {
            enemy_w = 15;
            enemy_y = 34;
        }

        // Check my tank hit with enemy (30x30 ,at Y=30) 
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_enemy.x, enemy_y, enemy_w, enemy_h))
        {
            //tank destroy
            static_tank.isExploding = true;
            static_tank.explosion_timer = 0;
            // APP_DBG(">> GAME OVER - TANK CRASHED!\n");
            static_enemy.isExploding = true;
            static_enemy.explosionTimer = 0;
            // APP_DBG(">> ENEMY DESTROYED!\n");
        }
    }
}

// 1. Kiểm tra đạn của Ta (Đại bác & Minigun) trúng BOSS
static void check_collision_bullets_with_boss()
{
    if (!static_boss.is_active || static_boss.is_exploding || static_boss.isDie) return;

    // --- Check đạn Đại bác trúng Boss ---
    if (static_cannon_bullet.is_active) {
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_boss.x, static_boss.y, 60, 36)) 
        {
            static_cannon_bullet.is_active = false; // Tắt đạn ta
            
            // Gửi thư báo Boss bị trúng đạn (Sát thương = 1)
            uint8_t dmg = 1;
            task_post_common_msg(TG_BOSS_TASK_ID, BOSS_HIT_SIG, &dmg, sizeof(uint8_t));
        }
    }

    // --- Check đạn Minigun trúng Boss ---
    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++) {
        if (minigun_pool[i].is_active) {
            if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                static_boss.x, static_boss.y, 60, 36)) 
            {
                minigun_pool[i].is_active = false; // Tắt viên đạn minigun
                
                uint8_t dmg = 1;
                task_post_common_msg(TG_BOSS_TASK_ID, BOSS_HIT_SIG, &dmg, sizeof(uint8_t));
                break; // Thoát vòng lặp khung hình này
            }
        }
    }
}

// 2. Kiểm tra Rocket của Boss hoặc chính thân hình Boss húc trúng Xe tăng ta
static void check_collision_tank_with_boss()
{
    if (!static_boss.is_active || static_tank.isExploding) return;

    // --- Trường hợp 1: Rocket của Boss bắn trúng Xe tăng ---
    if (static_boss.rocket.is_active) {
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_boss.rocket.x, static_boss.rocket.y, 17, 11)) 
        {
            static_boss.rocket.is_active = false; // Rocket nổ
            static_tank.isExploding = true;       // Xe tăng ta bay màu
            static_tank.explosion_timer = 0;
            // APP_DBG(">> TANK DESTROYED BY BOSS ROCKET! <<\n");
        }
    }

    // --- Trường hợp 2: Boss bay đè trực tiếp trúng Xe tăng ---
    if (check_collision(static_tank.x, 30, 30, 30,
                        static_boss.x, static_boss.y, 60, 36)) 
    {
        static_tank.isExploding = true;
        static_tank.explosion_timer = 0;
        // APP_DBG(">> TANK CRASHED BY BOSS! <<\n");
    }
}


// 3. Logic điều khiển khi nào Boss xuất hiện (Spawn Trigger) dựa vào Score
static void check_and_spawn_boss()
{
    extern score_object_t static_score; // extern lấy dữ liệu điểm hiện tại
    
    // Nếu điểm đạt từ 30 điểm trở lên VÀ Boss chưa từng xuất hiện/chưa chết thì gọi Boss ra!
    if (static_score.current_score >= 30 && !static_boss.is_active && !static_boss.isDie && !static_boss.is_exploding)
    {
        APP_DBG(">> BOSS_SPAWN_SIG!\n");
        task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_SPAWN_SIG);
    }
}

// Hàm kiểm tra đạn Minigun của Ta bắn chặn phá hủy Rocket của Boss
static void check_collision_minigun_with_boss_rocket()
{
    // Nếu Boss chưa active, hoặc Rocket của Boss chưa bắn ra thì không cần check
    if (!static_boss.is_active || !static_boss.rocket.is_active) return;

    // Duyệt qua toàn bộ pool đạn Minigun đang bay của Ta
    for (int i = 0; i < MAX_MINIGUN_BULLETS; i++)
    {
        if (minigun_pool[i].is_active)
        {
            // Check va chạm: Đạn minigun (2x1) đụng Rocket của Boss (17x11)
            if (check_collision(minigun_pool[i].x, minigun_pool[i].y, 2, 1,
                                static_boss.rocket.x, static_boss.rocket.y, 17, 11))
            {
                minigun_pool[i].is_active = false; // Viên đạn Minigun của ta biến mất

                // Giảm máu của quả Rocket Boss
                if (static_boss.rocket.hp > 0) {
                    static_boss.rocket.hp--;
                }

                // Nếu Rocket hết máu thì kích nổ/biến mất ngay trên không trung
                if (static_boss.rocket.hp == 0) {
                    static_boss.rocket.is_active = false;
                    // APP_DBG(">> PLAYER DESTROYED BOSS ROCKET! <<\n");
                }
                
                break; // Thoát vòng lặp khung hình này để xử lý viên đạn tiếp theo
            }
        }
    }
}


//  ================  DRAW GAME OBJECTS ================ //
static void view_scr_game()
{      
    mountain_draw(); 
    building_draw(); 
    ground_draw();   
    tank_draw();     
    enemy_draw();   
    tree_draw();     
    cannon_bullet_draw(); 
    minigun_bullet_draw();
    score_draw();
    boss_draw();
};

view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_game 
};

view_screen_t scr_game = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// ================LOGIC (EVENT HANDLER)======================== //
void scr_game_handle(ak_msg_t* msg)
{
    switch (msg -> sig)
    {
        case SCREEN_ENTRY:
        {
            APP_DBG(">> Entered TANK Screen Success!\n");
            // setup for objects in game
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_SETUP_SIG);
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_SETUP_SIG); 
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_SETUP_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_SETUP_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_SETUP_SIG); 
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_SETUP_SIG); 

            // --- THÊM 2 DÒNG NÀY ĐỂ RESET ĐIỂM VÀ BOSS ---
            task_post_pure_msg(TG_SCORE_TASK_ID, SCORE_SETUP_SIG);
            task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_SETUP_SIG);

            // timer for game loop
            timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60, TIMER_PERIODIC);
        }
        break;

        // game loop
        case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE: 
        {
            
            // Update tọa độ các object
            task_post_pure_msg(TG_GROUND_TASK_ID, GROUND_UPDATE_SIG);
            task_post_pure_msg(TG_TANK_TASK_ID, TANK_UPDATE_SIG);
            task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_UPDATE_SIG);
            
            // Chỉ update quái thường nếu Boss chưa xuất hiện hoặc chưa chết hẳn
            if (static_enemy.isExploding || (!static_boss.is_active && !static_boss.isDie)) {
                task_post_pure_msg(TG_ENEMY_TASK_ID, ENEMY_UPDATE_SIG); 
            }
            
            task_post_pure_msg(TG_BOSS_TASK_ID, BOSS_UPDATE_SIG); // Luôn update Boss
            
            task_post_pure_msg(TG_TREE_TASK_ID, TREE_UPDATE_SIG); 
            task_post_pure_msg(TG_BUILDING_TASK_ID, BUILDING_UPDATE_SIG); 
            task_post_pure_msg(TG_MOUNTAIN_TASK_ID, MOUNTAIN_UPDATE_SIG); 
            task_post_pure_msg(TG_MINIGUN_BULLET_TASK_ID, MINIGUN_BULLET_UPDATE_SIG);

            // Xử lý nã đạn
            minigun_fire();
            
            // Xử lý va chạm với quái thường
            if (!static_boss.is_active) {
                check_collision_cannon_bullet_with_enemy();
                check_collision_minigun_bullet_with_enemy();
                check_collision_tank_with_enemy();
            }
            
            // ==========================================
            // KHÚC CHÈN LOGIC BOSS CỦA LUONG Ở ĐÂY:
            // ==========================================
            check_and_spawn_boss();           // Tự động kiểm tra điểm để gọi Boss xuất kích
            check_collision_bullets_with_boss(); // Kiểm tra đạn ta bắn trúng Boss
            check_collision_tank_with_boss();    // Kiểm tra Boss hoặc Rocket bắn trúng Xe tăng ta
            check_collision_minigun_with_boss_rocket();
        }
        break;

        //when MODE button down and HOLD
        case AC_DISPLAY_BUTON_MODE_DOWN:
            if(!static_tank.isExploding) is_minigun_firing = true;//active minigun
            break;
        case AC_DISPLAY_BUTTON_MODE_HOLD:
             if(!static_tank.isExploding) is_minigun_firing = true;//active minigun
            break;

        //button MODE released
        case AC_DISPLAY_BUTON_MODE_RELEASED: 
            is_minigun_firing = false; // stop minigun
             if(!static_tank.isExploding) task_post_pure_msg(TG_CANNON_BULLET_TASK_ID, CANNON_BULLET_FIRE_SIG);//fire cannon
        break;

        //button UP released
        case AC_DISPLAY_BUTON_UP_RELEASED:
             if(!static_tank.isExploding) task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_FORWARD_SIG);//move tank forward
        break;

        //button DOWN released
        case AC_DISPLAY_BUTON_DOWN_RELEASED:
             if(!static_tank.isExploding) task_post_pure_msg(TG_TANK_TASK_ID, TANK_MOVE_BACKWARD_SIG);//move tank backward
        break;

        default:
            break;
    }
}
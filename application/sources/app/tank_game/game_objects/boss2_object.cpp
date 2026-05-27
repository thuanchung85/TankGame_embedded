#include "boss2_object.h"

boss2_t static_boss2;

//=========BOSS2=======//
// cannon hit BOSS2
static void check_collision_cannon_bullets_with_boss2()
{
    if (!static_boss2.is_active || static_boss2.is_exploding || static_boss2.isDie)
        return;

    // --- Check đạn Đại bác trúng Boss2 ---
    if (static_cannon_bullet.is_active)
    {
        if (check_collision(static_cannon_bullet.x, static_cannon_bullet.y, 5, 3,
                            static_boss2.x, static_boss2.y, 42, 34))
        {
            static_cannon_bullet.is_active = false; // Tắt đạn ta

            // Gửi thư báo Boss bị trúng đạn (Sát thương = 1)
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

    // --- Trường hợp 1: Rocket của Boss bắn trúng Xe tăng ---
    if (static_boss2.cannon_bullet.is_active)
    {
        if (check_collision(static_tank.x, 30, 30, 30,
                            static_boss2.cannon_bullet.x, static_boss2.cannon_bullet.y, 13, 5))
        {
            static_boss2.cannon_bullet.is_active = false; // Rocket nổ
            static_tank.isExploding = true;               // Xe tăng ta bay màu
            static_tank.explosion_timer = 0;
            // APP_DBG(">> TANK DESTROYED BY BOSS ROCKET! <<\n");
        }
    }

    // --- Trường hợp 2: Boss bay đè trực tiếp trúng Xe tăng ---
    if (check_collision(static_tank.x, 30, 30, 30,
                        static_boss2.x, static_boss2.y, 42, 34))
    {
        static_tank.isExploding = true;
        static_tank.explosion_timer = 0;
        // APP_DBG(">> TANK CRASHED BY BOSS! <<\n");
    }
}

// Boss2 Spawn when Score
 void check_and_spawn_boss2()
{
    extern score_object_t static_score; // extern lấy dữ liệu điểm hiện tại

    // Nếu điểm đạt từ 400 điểm trở lên VÀ Boss chưa từng xuất hiện/chưa chết thì gọi Boss ra!
    if (static_score.current_score >= 400 && !static_boss2.is_active && !static_boss2.isDie && !static_boss2.is_exploding)
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
                }

                break; // Thoát vòng lặp khung hình này để xử lý viên đạn tiếp theo
            }
        }
    }
}

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




void task_boss2_handle(ak_msg_t *msg)
{
    switch (msg->sig)
    {

    case BOSS2_SETUP_SIG:
    case BOSS2_RESET_SIG:
    {
        boss2_reset();
    }
    break;

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

            uint8_t boss2_score_bonus = 100;
            task_post_common_msg(TG_SCORE_TASK_ID, SCORE_ADD_SIG, &boss2_score_bonus, sizeof(uint8_t));
        }
    }
    break;

    case BOSS2_UPDATE_SIG:
    {
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

        if (static_boss2.isDie || !static_boss2.is_active)
            break;

        // =========================================================
        // LÀM LẠI AI DI CHUYỂN TIẾN LÙI CHO BOSS 2 MƯỢT MÀ:
        // =========================================================
        static int8_t boss2_dir_x = -1; // -1 là tiến sang trái, 1 là lùi sang phải

        // Cập nhật vị trí X theo hướng
        static_boss2.x += boss2_dir_x;

        // Kiểm tra chạm biên để đảo hướng đi
        if (static_boss2.x <= 60)
        {
            boss2_dir_x = 1; // Đụng cận trái (X=60) -> Đổi hướng bay lùi về bên phải
        }
        else if (static_boss2.x >= 120)
        {
            boss2_dir_x = -1; // Đụng cận phải (X=120) -> Đổi hướng bay tiến sang bên trái
        }

        // --- Logic Cooldown nạp đạn và Bắn cannon ---
        if (!static_boss2.cannon_bullet.is_active)
        {
            static_boss2.fire_cooldown_counter++;
            if (static_boss2.fire_cooldown_counter >= 68)
            {
                if (rand() % 2 == 0)
                {
                    static_boss2.cannon_bullet.is_active = true;
                    static_boss2.cannon_bullet.x = static_boss2.x;
                    static_boss2.cannon_bullet.y = static_boss2.y + 12; // Vị trí nòng súng Boss

                    // KHÓA MỤC TIÊU: Tâm của xe tăng thường ở khoảng Y = 30 đến 45
                    static_boss2.cannon_bullet.targetY = 40;

                    static_boss2.cannon_bullet.hp = 1;
                    APP_DBG(">> BOSS 2 FIRED CANNON AT TANK! <<\n");
                }
                static_boss2.fire_cooldown_counter = 0;
            }
        }

        // --- Cập nhật vị trí cannon bullet đang bay (Bẻ lái hướng vào Xe Tăng) ---
        if (static_boss2.cannon_bullet.is_active)
        {
            // 1. Đạn liên tục bay tốc độ cao sang bên trái hướng về xe tăng
            static_boss2.cannon_bullet.x -= 1; // Tăng tốc lên 2 cho đạn bay nhanh nguy hiểm hơn!

            // 2. Tự động bẻ lái trục Y để đâm thẳng vào Xe tăng của người chơi
            if (static_boss2.cannon_bullet.y < static_boss2.cannon_bullet.targetY)
            {
                static_boss2.cannon_bullet.y += 1; // Nếu đạn ở trên cao -> Chúc đầu đi xuống
            }
            else if (static_boss2.cannon_bullet.y > static_boss2.cannon_bullet.targetY)
            {
                static_boss2.cannon_bullet.y -= 1; // Nếu đạn ở dưới thấp -> Ngóc đầu đi lên
            }

            // 3. Kiểm tra biến mất khi bay khỏi màn hình
            if (static_boss2.cannon_bullet.x < -17 || static_boss2.cannon_bullet.y > 64 || static_boss2.cannon_bullet.y < -5)
            {
                static_boss2.cannon_bullet.is_active = false;
            }
        }

        // =========================================================
        // DI CƯ LOGIC VA CHẠM CỦA BOSS 2 VỀ ĐÂY:
        // =========================================================
        if (static_boss2.is_active && !static_boss2.isDie)
        {
            // 1. Tự check đạn đại bác trúng mình (Khỏi cần gọi qua Active Kernel)
            check_collision_cannon_bullets_with_boss2();
            // 2. Tự check Minigun bắn chặn phá hủy Rocket của mình
            check_collision_minigun_with_boss2_cannon_bullet();
            // 3. Tự check mình hoặc Rocket húc trúng Xe tăng ta
            check_collision_tank_with_boss2();
        }
    }
    break;

    default:
        break;
    }
}

void boss2_draw()
{
    if (static_boss2.cannon_bullet.is_active)
    {
        view_render.drawBitmap(static_boss2.cannon_bullet.x, static_boss2.cannon_bullet.y, bitmap_boss2_cannon_bullet, 13, 5, WHITE);
    }

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

    // 3. Vẽ hiệu ứng nổ hoành tráng khi Boss 2 cạn máu
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
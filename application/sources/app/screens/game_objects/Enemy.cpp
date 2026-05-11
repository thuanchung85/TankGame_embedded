#include "Enemy.h"

// Định nghĩa Constructor
Enemy::Enemy() {
    x = 130 + (rand() % 100);
    enemy_type = 0;
    isExploding = false;
    explosionTimer = 0;
}

// Hàm cập nhật trạng thái 
void Enemy::update() {

    if (isExploding) {
        explosionTimer++;
        if (explosionTimer > 9) { // Sau khoảng 0.5s (9 frame * 60ms)
            isExploding = false;
            explosionTimer = 0;
            // Sau khi nổ xong thì reset vị trí để xuất hiện quân địch mới
            x = 130 + (rand() % 100);
            enemy_type = rand() % 4;
        }
        return; // Đang nổ thì không di chuyển x-- nữa
    }

    switch (enemy_type)
    {
        case 0://xe tank
            //scroll trên mặt đất, cho nó di chuyển từ phải sang trái
            x--;
            // Nếu đã trôi hết qua trái màn hình
            if (x < -35) {
                x = 120 + (rand() % 100);
                enemy_type = rand() % 4;//random ra enemy type, tank, troop, airplane, mine.
            }        
            break;

        case 1:// máy bay
            //scroll trên mặt đất, cho nó di chuyển từ phải sang trái
            x--;
            // Nếu đã trôi hết qua trái màn hình
            if (x < -35) {
                x = 150 + (rand() % 100);  
                enemy_type = rand() % 4;//random ra enemy type, tank, troop, airplane, mine.
            }        
            break;

        case 2://mìn
            //scroll trên mặt đất, cho nó di chuyển từ phải sang trái
            x--;
            // Nếu đã trôi hết qua trái màn hình
            if (x < -35) {
                x = 140 + (rand() % 100);
                enemy_type = rand() % 4;//random ra enemy type, tank, troop, airplane, mine, rocket.
            }        
            break;

        case 3://troop
            //scroll trên mặt đất, cho nó di chuyển từ phải sang trái
            x -= 2;
            // Nếu đã trôi hết qua trái màn hình
            if (x < -35) {
                x = 135 + (rand() % 100); 
                enemy_type = rand() % 4;//random ra enemy type, tank, troop, airplane, mine, rocket.
            }    
            break;
    }
    
   
}

// Hàm vẽ Enemy
void Enemy::draw() {

    if (isExploding) {
        // Chia explosionTimer để tạo hiệu ứng hoạt hình
        if (explosionTimer < 3) {
            view_render.drawBitmap(x, 30, bitmap_bum, 28, 20, WHITE);
        } else if (explosionTimer < 6) {
            view_render.drawBitmap(x, 25, bitmap_bum2, 30, 26, WHITE);
        } else {
            view_render.drawBitmap(x, 20, bitmap_bum3, 30, 31, WHITE);
        }

    } else {
     
        switch (enemy_type)
        {
            case 0://xe tank
                    view_render.drawBitmap(x, 33, bitmap_enemy_tank, 25, 21, WHITE);
                    break;

            case 1:// máy bay
                    view_render.drawBitmap(x, 5, bitmap_enemy_air_plane, 25, 21, WHITE);
                    break;

            case 2://mìn
                    view_render.drawBitmap(x, 33, bitmap_enemy_mine, 22, 21, WHITE);
                    break;

            case 3://troop
                view_render.drawBitmap(x, 34, bitmap_enemy_troop, 15, 21, WHITE);
                break;
        }
    }
}

bool Enemy::checkCollision(short bulletX, short bulletY, short bulletW, short bulletH) {
    short enemyW, enemyH, enemyY;
    
    // 1. Xác định kích thước vùng va chạm tùy theo loại địch
    switch (enemy_type) {
        case 0: enemyW = 25; enemyH = 21; enemyY = 33; break; // Tank
        case 1: enemyW = 25; enemyH = 21; enemyY = 5;  break; // Air
        case 2: enemyW = 22; enemyH = 21; enemyY = 33; break; // Mine
        case 3: enemyW = 15; enemyH = 21; enemyY = 34; break; // Troop
        default: return false;
    }

    // 2. Thuật toán kiểm tra hình chữ nhật giao nhau
    if (bulletX < x + enemyW &&
        bulletX + bulletW > x &&
        bulletY < enemyY + enemyH &&
        bulletY + bulletH > enemyY) {
        return true; // Có va chạm!
    }
    
    return false;
}
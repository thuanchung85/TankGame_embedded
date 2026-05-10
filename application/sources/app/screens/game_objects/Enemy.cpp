#include "Enemy.h"

// Định nghĩa Constructor
Enemy::Enemy() {
    x = 130 + (rand() % 100);
    enemy_type = 0;
}

// Hàm cập nhật trạng thái 
void Enemy::update() {

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






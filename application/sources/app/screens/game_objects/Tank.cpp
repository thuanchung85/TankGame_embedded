#include "Tank.h"
extern "C" {
    #include "../../../../driver/buzzer/buzzer.h"
}

// Định nghĩa Constructor
Tank::Tank() {
    reset();
}

void Tank::reset(){
    x = 20;
    target_x = 20;
    isMoving = false;
    isExploding = false;
    explosion_timer = 0;
    myHP.reset();
}

// Hàm cập nhật trạng thái 
void Tank::update() {
    if(!isExploding)//nếu tank còn sống
    { 
        if(isMoving == true)
        {
            if (x < target_x) x += 1;

            if (x > target_x) x -= 1;

            if(x == target_x) isMoving = false;
        }
    

        //cứ 50 frame bắn 1 viên canon
        frame_count++;
        if(frame_count >= 50)
        {
            tank_fire_canon();
            frame_count = 0;
            
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            
        }

        // update các viên đạn của tank
        my_canon_bullets.update();
        my_gun_bullets.update();

        //HP update
        myHP.update();
    }
    else// nếu tank nổ tung
    {
        explosion_timer++;
        if(explosion_timer > 20) { // Sau khoảng 1.2s nổ thì thực hiện reset hoặc Game Over
            // Gọi lệnh quay về màn hình Menu hoặc Restart game ở đây
        }
    }
}

// Hàm vẽ tank
void Tank::draw() {
    if (isExploding) {
        // Vẽ chuỗi bitmap nổ giống Enemy (bum, bum2, bum3)
        if (explosion_timer < 5) view_render.drawBitmap(x, 30, bitmap_bum, 28, 20, WHITE);
        else if (explosion_timer < 10) view_render.drawBitmap(x, 30, bitmap_bum2, 30, 26, WHITE);
        // ...
    }
    else
    {
        //Lệnh này vẽ hình ảnh tank lên màn hình. 
        // 20, 30: Tọa độ bắt đầu (góc trên cùng bên trái).
        // bitmap_tank: Dữ liệu hình ảnh lấy từ screens_bitmap.h mà bạn thấy trong file header.
        // 30, 30: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60). tank size 30x30
        // WHITE: Màu sắc của hình ảnh.
        view_render.drawBitmap(x, 30, bitmap_tank, 30, 30, WHITE);

        //vẽ đạn của tank
        my_canon_bullets.draw();
        my_gun_bullets.draw();
        
        //vẽ HP
        myHP.draw();
    }
}


// Hàm xử lý khi tank bắn canon
void Tank::tank_fire_canon() {
   
        if (!my_canon_bullets.is_active) {
            // Bắn ra từ vị trí đầu nòng pháo (khoảng x+30, y+5 của xe tank)
            my_canon_bullets.fire(x + 25, 38); 
        }

}
// Hàm xử lý khi tank bắn gun
void Tank::tank_fire_gun(short enemyY) {
    if (!my_gun_bullets.is_active) {
        // Truyền thêm enemyY để viên đạn biết hướng bay
        my_gun_bullets.fire(x + 18, 31, enemyY); 
    }
}

void Tank::moveForward() {
    if(isMoving == false)
    {
        if (x < 100-30) { // Giới hạn mép phải màn hình (trừ đi chiều rộng tank)
            target_x += 10; // Chỉ thay đổi con số, rất nhẹ cho RAM
            isMoving = true;
        }
    }
   
}

void Tank::moveBackward() {
    if(isMoving == false){
        if (x > 0) { // Giới hạn mép trái màn hình
            target_x -= 10; // Chỉ thay đổi con số, rất nhẹ cho RAM
            isMoving = true;
        }
    }
   
}

// hàm va chạm với địch
bool Tank::checkCollisionWithEnemy(short enemyX, short enemyY, short enemyW, short enemyH) {
    int8_t tankW = 25; // Chiều rộng xe tank
    int8_t tankH = 15; // Chiều cao xe tank
    int8_t tankY = 30; // Tọa độ Y cố định của tank trên mặt đất

    // Kiểm tra hình chữ nhật giao nhau giữa Tank và Enemy
    if (x < enemyX + enemyW &&
        x + tankW > enemyX &&
        tankY < enemyY + enemyH &&
        tankY + tankH > enemyY) {
        return true; 
    }
    return false;
}

void Tank::lossHP(){
    myHP.subHP();
    if(myHP.hpPoint <= 0)
    {
        isExploding = true;
    }
}
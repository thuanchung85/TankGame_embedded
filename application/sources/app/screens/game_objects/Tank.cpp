#include "Tank.h"

// Định nghĩa Constructor
Tank::Tank() {
    x = 20;
    target_x = 20;
    isMoving = false;
}

// Hàm cập nhật trạng thái 
void Tank::update() {

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
    }
    
    frame_count2++;
    if(frame_count2 >= 30)
    {
        tank_fire_gun();
        frame_count2 = 0;
    }

    // update các viên đạn của tank
    my_canon_bullets.update();
    my_gun_bullets.update();

    //HP update
    myHP.update();
}

// Hàm vẽ tank
void Tank::draw() {
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


// Hàm xử lý khi tank bắn canon
void Tank::tank_fire_canon() {
   
        if (!my_canon_bullets.is_active) {
            // Bắn ra từ vị trí đầu nòng pháo (khoảng x+30, y+5 của xe tank)
            my_canon_bullets.fire(x + 25, 38); 
        }

}
// Hàm xử lý khi tank bắn gun
void Tank::tank_fire_gun() {
   
        if (!my_gun_bullets.is_active) {
            // Bắn ra từ vị trí đầu nòng gun
            my_gun_bullets.fire(x + 18, 31); 
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
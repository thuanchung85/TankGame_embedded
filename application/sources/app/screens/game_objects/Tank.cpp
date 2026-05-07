#include "Tank.h"

// Định nghĩa Constructor
Tank::Tank() {
    x = 20;
    speed = 0;
}

// Hàm cập nhật trạng thái 
void Tank::update() {

    //cứ 10 frame bắn 1 viên
    frame_count++;
    if(frame_count >= 50)
    {
        tank_fire();
        frame_count = 0;
    }

    // update các viên đạn của tank
    my_bullets.update();

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
    my_bullets.draw();
    
    //vẽ HP
    myHP.draw();
}


// Hàm xử lý khi bấm nút bắn (ví dụ nút UP hoặc nút OK)
void Tank::tank_fire() {
   
        if (!my_bullets.is_active) {
            // Bắn ra từ vị trí đầu nòng pháo (khoảng x+30, y+5 của xe tank)
            my_bullets.fire(x + 25, 38); 
        }
    
}

#include "Tank.h"

// Định nghĩa Constructor
Tank::Tank() {
    x = 20;
    speed = 0;
}

// Hàm cập nhật trạng thái 
void Tank::update() {
    // Bạn có thể thêm logic rung lắc khi chạy ở đây
    // Hoặc logic nhảy (jump) nếu muốn
}

// Hàm vẽ tank
void Tank::draw() {
    //Lệnh này vẽ hình ảnh tank lên màn hình. 
    // 20, 30: Tọa độ bắt đầu (góc trên cùng bên trái).
    // bitmap_tank: Dữ liệu hình ảnh lấy từ screens_bitmap.h mà bạn thấy trong file header.
    // 30, 30: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60). tank size 30x30
    // WHITE: Màu sắc của hình ảnh.
    view_render.drawBitmap(x, 30, bitmap_tank, 30, 30, WHITE);
}


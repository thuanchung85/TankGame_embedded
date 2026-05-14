#include "Mountain.h"

// Định nghĩa Constructor
Mountain::Mountain() {
   reset();    
}
void Mountain::reset(){
    x =200;
}
// Hàm cập nhật trạng thái 
void Mountain::update() {
    //scroll núi trên mặt đất, cho nó di chuyển từ phải sang trái
    //núi ở xa chạy chậm hơn mặt đất và cây dừa 6 frame.
    frame_count++;
    if(frame_count >= 6)
    {
        x--;
        if (x < -120) 
        {
            x = 200 + 90; // Reset về bên phải để xuất hiện lại
        }
        frame_count = 0;
    }
}

// Hàm vẽ 
void Mountain::draw() {
    // x, 0: Tọa độ bắt đầu.
    // bitmap_theMountain: Dữ liệu hình ảnh lấy từ screens_bitmap.h
    // 90, 42: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60).
    // WHITE: Màu sắc của hình ảnh.
    view_render.drawBitmap(x, 0, bitmap_theMountain, 90, 42, WHITE);
}


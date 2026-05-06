#include "Ground.h"

// Định nghĩa Constructor
Ground::Ground() {
    scroll_offset = 20;
}

// Hàm cập nhật trạng thái 
void Ground::update() {
    //scroll mặt đất
    scroll_offset++;
    if (scroll_offset >= 24) { // 24 là độ rộng mảng ground của bạn
        scroll_offset = 0;
    }
}

// Hàm vẽ 
void Ground::draw() {
    // Vẽ mặt đất bằng các khối 24x8 pixel
    // Tọa độ y = 52 (nằm dưới chân xe tank)
    for (short x = 0; x < 124; x += 24) { 
        // Lưu ý: chiều rộng là 24, chiều cao là 8
        view_render.drawBitmap(x - (scroll_offset % 24), 52, bitmap_ground, 24, 8, WHITE);
    }
}


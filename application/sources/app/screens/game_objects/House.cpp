#include "House.h"

// Định nghĩa Constructor
House::House() {
    x = 180; 
    house_type = 0; // Mặc định là nhà số 1
}

// Hàm cập nhật trạng thái 
void House::update() {
    frame_count++;
    if(frame_count >= 2)
    {
        x--;
        if (x < -120) 
        {
            x = 130 + (rand() % 100); // Reset về bên phải để xuất hiện lại

            // --- PHẦN RANDOM QUAN TRỌNG ---
            // rand() % 4 sẽ trả về một số ngẫu nhiên từ 0, 1, 2, 3
            house_type = rand() % 4;
        }
        frame_count = 0;
    }
}

// Hàm vẽ 
void House::draw() {
    // Mảng chứa các bitmap nhà của bạn
    const unsigned char* house_bitmaps[] = {
        bitmap_house1, 
        bitmap_house2, 
        bitmap_house3, 
        bitmap_house4
    };
    view_render.drawBitmap(x, 10, house_bitmaps[house_type], 30, 30, WHITE);

}


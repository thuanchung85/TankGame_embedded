#include "Score.h"

// Định nghĩa Constructor
Score::Score() {
   reset();
}

void Score::reset(){
    current_score = 0;
}

// Hàm cập nhật trạng thái 
void Score::update() {
   /*
     frame_count++;
    if(frame_count >= 100)
    {
        current_score++;
        frame_count = 0;
    }
   */
}

void Score::add() {
   current_score+=10;
}

// Hàm vẽ 
void Score::draw() {
    // 1. Thiết lập vị trí hiển thị (Góc trên bên phải màn hình 124x60)
    // x=110 để không đè lên trái tim HP ở bên trái
    view_render.setCursor(110, 1); 
    
    // 2. Thiết lập màu sắc và kích thước
    view_render.setTextSize(1);      // Kích thước nhỏ nhất (6x8px mỗi ký tự)
    view_render.setTextColor(WHITE); // Chữ màu trắng
    
    // 3. Hiển thị chữ "S:" và con số
    // Bạn có thể dùng một hàm hỗ trợ để chuyển số sang chuỗi
    // Hoặc nếu view_render hỗ trợ printf thì càng tốt
    char scoreStr[6];
    sprintf(scoreStr, "%d", current_score);
    
    for(int i = 0; scoreStr[i] != '\0'; i++) {
        view_render.write(scoreStr[i]); 
    }

}


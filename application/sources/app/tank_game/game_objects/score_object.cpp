#include "score_object.h"

// Khởi tạo vùng nhớ tĩnh ban đầu cho Điểm số
score_object_t static_score = {
    .current_score = 0
};

void task_score_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case SCORE_SETUP_SIG:
        case SCORE_RESET_SIG: {
            static_score.current_score = 0;
        } break;

        case SCORE_ADD_SIG: {
            // 1. Dùng hàm API của kernel để lấy con trỏ trỏ tới mảng data
            uint8_t* msg_data = get_data_common_msg(msg); 
            
            // 2. Lấy giá trị uint8_t đầu tiên trong mảng dữ liệu đó ra ngoài
            uint8_t points_to_add = msg_data[0]; 
            
            // 3. Cộng điểm số vào
            static_score.current_score += points_to_add;
            
            APP_DBG(">> Score Updated: +%d | Total: %ld\n", points_to_add, static_score.current_score);
        } break;

        default:
            break;
    }
}

void score_draw() {
    // Đặt con trỏ ở góc trên bên phải màn hình (X=105 cho rộng rãi, Y=1)
    view_render.setCursor(105, 1); 
    view_render.setTextSize(1);     
    view_render.setTextColor(WHITE); 
    
    char scoreStr[7];
    sprintf(scoreStr, "%ld", static_score.current_score);
    
    // In chuỗi điểm ra màn hình OLED giống hệt code cũ của bạn
    for(int i = 0; scoreStr[i] != '\0'; i++) {
        view_render.write(scoreStr[i]); 
    }
}
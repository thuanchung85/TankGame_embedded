#ifndef __GROUND_OBJECT_H__
#define __GROUND_OBJECT_H__

#include <stdint.h>
#include "task_list.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"
#include "app_dbg.h"

// Forward declaration: Báo cho trình biên dịch biết cấu trúc tin nhắn của AK mà không cần include 
struct ak_msg_t;

// Định nghĩa các Tín hiệu (Signal) dành riêng cho Mặt Đất
enum {
    GROUND_SETUP_SIG = 1,  // Cấu hình ban đầu cho mặt đất
    GROUND_UPDATE_SIG,     // Lệnh dịch chuyển mặt đất (cuộn)
    GROUND_RESET_SIG       // Reset lại trạng thái khi chơi lại
};

// Cấu trúc dữ liệu lưu trạng thái mặt đất
typedef struct {
    int8_t x;  // Tọa độ X để tạo hiệu ứng cuộn màn hình
} ground_t;

// Khai báo extern để file màn hình (scr_game.cpp) có thể vào đọc tọa độ X đi vẽ
extern ground_t static_ground;

#ifdef __cplusplus
extern "C" {
#endif

// Hàm Entry Point (Xử lý sự kiện) của Task Mặt Đất
void task_ground_handle(struct ak_msg_t *msg);

//Hàm vẽ bitmap của mặt đất
void ground_draw();

#ifdef __cplusplus
}
#endif

#endif // __GROUND_OBJECT_H__
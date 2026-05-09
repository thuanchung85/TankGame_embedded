/*
Header Guard (#ifndef, #define, #endif) Đây là lớp "giáp" bảo vệ file:
Mục đích: Đảm bảo file này chỉ được đọc một lần duy nhất bởi trình biên dịch, ngay cả khi bạn #include nó ở nhiều nơi khác nhau.
Cách hoạt động: Nếu cái tên __SCR_TANK_H__ chưa được định nghĩa (ifndef), nó sẽ định nghĩa ngay lập tức và đọc nội dung bên dưới. Lần sau nếu gặp lại, nó sẽ bỏ qua để tránh lỗi định nghĩa chồng chéo.
*/
#ifndef __SCR_GAME_H__
#define __SCR_GAME_H__

/*
Các lệnh #include. Phần này khai báo những "nguyên liệu" mà Tank cần để hoạt động:
fsm.h: Quản lý trạng thái (Finite State Machine) của nhân vật.
message.h: Để nhận và xử lý các tin nhắn (messages/events) trong hệ thống AK.
task_display.h & view_render.h: Các công cụ để vẽ hình ảnh lên màn hình.
screens_bitmap.h: Chứa dữ liệu hình ảnh (pixels) của nhân vật Tank.
*/
#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "view_render.h"
#include "bitmap_data/screens_bitmap.h"

/*
Từ khóa extern (Lời hứa của Header). Đây là phần thú vị nhất. extern nói với trình biên dịch rằng: "Những thứ này tồn tại đâu đó trong các file .cpp, đừng lo, cứ cho phép dùng đi, lúc chạy sẽ thấy!"

extern view_dynamic_t dyn_view_tank;: Khai báo một vùng hiển thị động cho tank.

extern view_screen_t scr_tank;: Khai báo cấu trúc màn hình chính của tank.

extern void scr_tank_handle(ak_msg_t* msg);: Đây là hàm xử lý logic. Nó sẽ quyết định tank làm gì khi có tin nhắn đến (ví dụ: bấm nút thì bắn đậu).
*/
extern view_dynamic_t dyn_view_game;
extern view_screen_t scr_game;
extern void scr_game_handle(ak_msg_t* msg);

#endif 
#include "Tree.h"

// Định nghĩa Constructor
Tree::Tree() {
    x = 130; 
    tree_type = 0;
}

// Hàm cập nhật trạng thái 
void Tree::update() {
    //scroll cây dừa trên mặt đất, cho nó di chuyển từ phải sang trái
    x--;
    // Nếu cây dừa đã trôi hết qua trái màn hình
    if (x < -35) {
        x = 124 + 35; // Reset về bên phải để xuất hiện lại

        tree_type = rand() % 3;
    }
}

// Hàm vẽ 
void Tree::draw() {
    const unsigned char* tree_bitmaps[] = {
            bitmap_coconutTree, 
            bitmap_dryTree, 
            bitmap_grassesTree
        };

        // x, 52 - 31: Tọa độ bắt đầu.
        // bitmap_coconuttree: Dữ liệu hình ảnh lấy từ screens_bitmap.h  
        // 30, 31: Kích thước vùng vẽ (độ phân giải màn hình của bạn là 124x60).
        // WHITE: Màu sắc của hình ảnh.
        view_render.drawBitmap(x, 52 - 31, tree_bitmaps[tree_type], 30, 31, WHITE);
}


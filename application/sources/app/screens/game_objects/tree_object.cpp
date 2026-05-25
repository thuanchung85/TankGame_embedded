#include "tree_object.h"

tree_t static_tree = {
    .x = 130,        // Đặt sẵn nó ở ngoài màn hình bên phải
    .tree_type = 0   // Loại cây đầu tiên
};

const unsigned char* tree_bitmaps[] = {
            bitmap_coconutTree, 
            bitmap_dryTree, 
            bitmap_grassesTree
        };

void task_tree_handle(ak_msg_t *msg) {
    switch (msg->sig) {
        
        case TREE_SETUP_SIG:
        case TREE_RESET_SIG: {
            static_tree.x = 130; 
            static_tree.tree_type = 0;
        } break;

        case TREE_UPDATE_SIG: {
            static_tree.x--; 
            if (static_tree.x < -35) {
                static_tree.x = 124 + 35; 
                static_tree.tree_type = rand() % 3;
            }
        } break;

        default:
            break;
    }
}

void tree_draw() {
   
    view_render.drawBitmap(static_tree.x, 52 - 31, tree_bitmaps[static_tree.tree_type], 30, 31, WHITE);

}
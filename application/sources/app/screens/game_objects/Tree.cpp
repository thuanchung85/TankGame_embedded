#include "Tree.h"

Tree::Tree() {
    reset();
}
void Tree::reset(){
    x = 130; 
    tree_type = 0;
}

void Tree::update() {
    x--;
    if (x < -35) {
        x = 124 + 35; 
        tree_type = rand() % 3;
    }
}

void Tree::draw() {
    const unsigned char* tree_bitmaps[] = {
            bitmap_coconutTree, 
            bitmap_dryTree, 
            bitmap_grassesTree
        };

    view_render.drawBitmap(x, 52 - 31, tree_bitmaps[tree_type], 30, 31, WHITE);
}


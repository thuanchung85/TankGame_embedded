#ifndef TREE_H
#define TREE_H

#include "../bitmap_data/screens_bitmap.h"

class Tree {
public:
    short x;//vị trí mọc cây trên đất
    int8_t tree_type;//có 3 dạng cây, cây dừa, cây khô, cây bụi
    
    Tree();         // Khai báo Constructor
    void update();  // Khai báo hàm cập nhật
    void draw();    // Khai báo hàm vẽ
};

#endif
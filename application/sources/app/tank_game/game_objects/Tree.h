#ifndef TREE_H
#define TREE_H

#include "../bitmap_data/screens_bitmap.h"

class Tree {
public:
    short x;
    int8_t tree_type;
    
    Tree();         
    void update();  
    void draw();    
    void reset();
};

#endif
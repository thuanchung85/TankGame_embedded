#include "Ground.h"

Ground::Ground() {
    scroll_offset = 20;
}

void Ground::update() {
    scroll_offset++;
    if (scroll_offset >= 24) {
        scroll_offset = 0;
    }
}

void Ground::draw() {
    for (short x = 0; x < 124; x += 24) { 
        view_render.drawBitmap(x - (scroll_offset % 24), 52, bitmap_ground, 24, 8, WHITE);
    }
}


#include "Mountain.h"

Mountain::Mountain() {
   reset();    
}
void Mountain::reset(){
    x =200;
}
void Mountain::update() {
    frame_count++;
    if(frame_count >= 6)
    {
        x--;
        if (x < -120) 
        {
            x = 200 + 90; 
        }
        frame_count = 0;
    }
}

void Mountain::draw() {
    view_render.drawBitmap(x, 0, bitmap_theMountain, 90, 42, WHITE);
}


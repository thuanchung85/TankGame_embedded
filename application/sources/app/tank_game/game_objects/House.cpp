#include "House.h"

House::House() {
   reset();
}
void House::reset(){
     x = 180; 
    house_type = 0; 
}

void House::update() {
    frame_count++;
    if(frame_count >= 2)
    {
        x--;
        if (x < -120) 
        {
            x = 130 + (rand() % 100); 
            house_type = rand() % 4;
        }
        frame_count = 0;
    }
}

void House::draw() {
    const unsigned char* house_bitmaps[] = {
        bitmap_house1, 
        bitmap_house2, 
        bitmap_house3, 
        bitmap_house4
    };
    view_render.drawBitmap(x, 10, house_bitmaps[house_type], 30, 30, WHITE);

}


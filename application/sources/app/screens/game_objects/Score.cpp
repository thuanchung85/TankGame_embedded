#include "Score.h"

Score::Score() {
   reset();
}

void Score::reset(){
    current_score = 0;
}

void Score::update() {
  
}

void Score::add() {
   current_score+=10;
}

void Score::draw() {
    view_render.setCursor(110, 1); 
    
    view_render.setTextSize(1);      
    view_render.setTextColor(WHITE); 
    
    char scoreStr[6];
    sprintf(scoreStr, "%ld", current_score);
    
    for(int i = 0; scoreStr[i] != '\0'; i++) {
        view_render.write(scoreStr[i]); 
    }

}


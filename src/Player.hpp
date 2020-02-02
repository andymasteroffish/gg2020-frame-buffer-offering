//
//  Player.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#ifndef Player_hpp
#define Player_hpp

#include "ofMain.h"

class Player{
public:
    
    void setup(float start_x, float start_y);
    void move(int x_dir, int y_dir);
    void draw();
    
    ofVec2f pos;
    
    float move_speed;
    
};

#endif /* Player_hpp */

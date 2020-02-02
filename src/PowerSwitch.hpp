//
//  PowerSwitch.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#ifndef PowerSwitch_hpp
#define PowerSwitch_hpp

#include "ofMain.h"

class PowerSwitch{
public:
  
    void setup(float x, float y);
    void update(ofVec2f player_pos);
    void draw();
    
    ofVec2f pos;
    
    bool is_active;
    float hit_range;
    
    bool can_toggle;
};

#endif /* PowerSwitch_hpp */

//
//  Goal.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#ifndef Goal_hpp
#define Goal_hpp

#include "ofMain.h"

class Goal{
public:
    
    void setup(float x, float y, float w, float h);
    void update();
    void draw_fbo();
    void draw_top();
    
    ofRectangle box;
    
};

#endif /* Goal_hpp */

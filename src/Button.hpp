//
//  Button.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/2/20.
//

#ifndef Button_hpp
#define Button_hpp

#include "ofMain.h"

class Button{
public:
   
    void setup(string _text, int target, int _center_x, int _center_y, int w, int h, ofTrueTypeFont * _font);
    void update();
    void draw_fbo();
    void draw_top();
    
    void mouseMoved(int x, int y );
    void mousePressed(int x, int y);
    bool mouseReleased(int x, int y);
    
    ofColor col;
    ofRectangle box;
    int center_x, center_y;
    ofTrueTypeFont * font;
    
    int target_level;
    
    bool mouse_over;
    bool can_click;
    
    bool is_level_select;
    
    string text;
};

#endif /* Button_hpp */

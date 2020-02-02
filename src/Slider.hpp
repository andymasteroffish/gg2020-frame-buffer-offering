//
//  Slider.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#ifndef Slider_hpp
#define Slider_hpp

#include "ofMain.h"

class Slider{
public:
  
    void setup(string _name, float pos_x, float pos_y, int size );
    void set_audio(string name_a, string name_b);
    void set_mute(bool muted);
    void set_x(string label, float * _val, float min, float max, float default_val);
    void set_y(string label, float * _val, float min, float max, float default_val);
    void set_to_defaults();
    void mouse_pressed(int mouseX, int mouseY);
    void update(int mouseX, int mouseY);
    void update_values();
    void draw();
    string float2string(float val);
    
    
    string name;
    string label_x, label_y;
    
    float * val_x, * val_y;
    float min_x, max_x, default_x;
    float min_y, max_y, default_y;
    
    ofVec2f top_left;
    int box_size;
    
    ofVec2f cursor;
    ofVec2f default_cursor;
    
    bool can_move;
    
    //testig this as the source of audio
    ofSoundPlayer x_clip, y_clip;
    float volume_min, volume_max;
    
};

#endif /* Slider_hpp */

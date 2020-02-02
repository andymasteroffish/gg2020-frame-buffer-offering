//
//  Particle.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#ifndef Particle_hpp
#define Particle_hpp

#include "ofMain.h"
#include "Goal.hpp"

class Particle{
public:
    
    void setup(float x, float y);
    void update(ofPixels * pix);
    void draw();
    
    void kill();
    void save(Goal * goal);
    
    float getHueMatchPrc(ofColor col);
    ofVec2f getForceFromHue();
    ofVec2f getForceFromBri();
  
    ofVec2f pos;
    ofVec2f vel;
    
    float fric;
    
#define BOX_SIZE 5
    ofColor box_cols[BOX_SIZE][BOX_SIZE];
    float hue_match[BOX_SIZE][BOX_SIZE];
    
    float target_hue;
    float target_sat;
    float target_bri;
    
    //animations
    bool anim_done;
    bool is_saved, is_dead;
    
    float anim_timer;
    
    ofVec2f anim_target_pos;
};

#endif /* Particle_hpp */

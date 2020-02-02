//
//  ParticleSystem.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#ifndef ParticleSystem_hpp
#define ParticleSystem_hpp

#include "ofMain.h"
#include "Particle.hpp"
#include "Goal.hpp"

class ParticleSystem{
public:
    
    void setup(int w, int h, Goal * _goal, int level_num);
    void update(ofFbo * fbo);
    void push_particles_from_eahcother();
    void draw();
    
    int width, height;
    Goal * goal;
    
    vector<Particle> particles;
    vector<Particle> animating_particles;
    
    int saved_particles, dead_particles;
    
    bool saved_flag, dead_flag;
    
};

#endif /* ParticleSystem_hpp */

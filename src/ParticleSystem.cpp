//
//  ParticleSystem.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#include "ParticleSystem.hpp"

void ParticleSystem::setup(int w, int h,  Goal * _goal, int level_num){
    width = w;
    height = h;
    goal = _goal;
    
    particles.clear();
    animating_particles.clear();
    
    saved_flag = false;
    dead_flag = false;
    
    int num_particles = 30;
    
    //no particles on menu
    if (level_num < 0){
        num_particles = 0;
    }
    
    for (int i=0; i<num_particles; i++){
        Particle p;
        float range = ofRandom(10,100);
        ofVec2f anchor = ofVec2f(width/2, height/2);
        
        if (level_num == 3){
            anchor.y = height * 0.35;
            anchor.x = i % 2 == 0 ? width*0.65 : width*0.35;
            range = ofRandom(10,50);
        }
        
        float angle = ofRandom(TWO_PI);
        p.setup(anchor.x + cos(angle)*range, anchor.y+ sin(angle)*range);
        particles.push_back(p);
    }
    
    saved_particles = 0;
    dead_particles = 0;
    
}

void ParticleSystem::update(ofFbo * fbo){
    ofPixels pix;
    fbo->readToPixels(pix);
    
    for (int i=particles.size()-1; i>=0; i--){
        particles[i].update(&pix);
        
        //out of bounds = dead
        if (particles[i].pos.x < 0 || particles[i].pos.x >= width || particles[i].pos.y < 0 || particles[i].pos.y >= height ){
            particles[i].kill();
            animating_particles.push_back(particles[i]);
            particles.erase(particles.begin()+i);
            dead_particles++;
            dead_flag = true;
        }
        
        //in goal = saves
        if (goal->box.inside(particles[i].pos.x, particles[i].pos.y)){
            particles[i].save(goal);
            animating_particles.push_back(particles[i]);
            particles.erase(particles.begin()+i);
            saved_particles++;
            saved_flag = true;
        }
    }
    
    push_particles_from_eahcother();
    
    for (int i=animating_particles.size()-1; i>=0; i--){
        animating_particles[i].update(&pix);
        if (animating_particles[i].anim_done){
            animating_particles.erase(animating_particles.begin()+i);
        }
    }
}

void ParticleSystem::push_particles_from_eahcother(){
    float range = 5;
    float power = 0.03;
    for (int i=0; i<particles.size(); i++){
        for (int k=0; k<i; k++){
            ofVec2f dif = particles[i].pos - particles[k].pos;
            float dist = dif.length();
            if (dist < range){
                float prc = 1.0-dist/range;
                float force = power * prc;
                ofVec2f push = dif.getNormalized();
                push *= force;
                //cout<<"force "<<force<<endl;
                particles[k].vel -= force;
                particles[i].vel += force;
            }
        }
    }
}

void ParticleSystem::draw(){
    for (int i=0; i<particles.size(); i++){
        particles[i].draw();
    }
    for (int i=0; i<animating_particles.size(); i++){
        animating_particles[i].draw();
    }
}

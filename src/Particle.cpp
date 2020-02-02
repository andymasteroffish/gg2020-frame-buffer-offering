//
//  Particle.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#include "Particle.hpp"

void Particle::setup(float x, float y){
    pos.set(x, y);
    vel.set(0,0);
    
    fric = 0.1;
    
    is_saved = false;
    is_dead = false;
    
    ofColor targetCol;
    targetCol.set(255, 117, 241);
    target_hue = targetCol.getHue();
    target_sat = targetCol.getSaturation();
    target_bri = targetCol.getBrightness();
    
    anim_done = false;
}

void Particle::update(ofPixels * pix){
    
    //get the box around our pixel
    int home_x = floor( pos.x );
    int home_y = floor( pos.y );
    for (int c=0; c<BOX_SIZE; c++){
        for (int r=0; r<BOX_SIZE; r++){
            //assume black
            box_cols[c][r].set(0, 0, 0);
            
            //if it's in range, set it based on the pixels
            int x = home_x + (c-2);
            int y = home_y + (r-2);
            
            if (x >= 0 && x < pix->getWidth() && y >=0 && y < pix->getHeight()){
                box_cols[c][r] = pix->getColor(x, y);
            }
            
            //get the hue match
            hue_match[c][r] = getHueMatchPrc(box_cols[c][r]);
        }
    }
    
    
    if (!is_saved && !is_dead){
        vel *= (1.0-fric);
        
        vel += getForceFromBri();
        vel += getForceFromHue();
        
        pos += vel;
    }
    
    if (is_saved){
        float zeno = 0.1;
        pos = (1.0-zeno) * pos + zeno * anim_target_pos;
        if (ofDist(pos.x,pos.y, anim_target_pos.x, anim_target_pos.y) < 22){
            anim_done = true;
        }
    }
    
    
    if (is_dead){
        vel.set(0,0);
        float range = 5;
        pos.x += ofRandom(-range, range);
        pos.y += ofRandom(-range, range);
        if (anim_timer > 1){
            anim_done = true;
        }
    }
    
    anim_timer += ofGetLastFrameTime();
}

void Particle::draw(){
    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y);
    
    ofColor col(250,230,10);
    if (is_dead){
        col.set(255,10,10);
    }
    
    //halo
    float padding = 0.5;
    float speed = 0.5;
    if (is_saved || is_dead){
        padding = 0;
        speed = 1;
    }
    float halo_prc = fmod(ofGetElapsedTimef()*speed, 1+padding) - padding;
    halo_prc = CLAMP(halo_prc, 0, 1);
    float halo_a = 150 * (1.0-halo_prc);
    float halo_size = halo_prc * 20;
    if (is_saved || is_dead){
        halo_size += anim_timer * 6;
    }
//    if (is_dead){
//        halo_size = 0;
//    }
    ofSetColor(col.r, col.g, col.b, halo_a);
    ofFill();
    ofDrawCircle(0, 0, halo_size);
    //cout<<"a "<<halo_a<<"  size "<<halo_size<<endl;
    
    //main dot
    ofSetColor(col);
    ofFill();
    float radius = 2;
    if (is_saved){
        radius += anim_timer * 2;
    }
    ofDrawCircle(0, 0, radius);
    
    //the force vector
    //float demo_scale = 10;
    //ofSetColor(0);
    //ofVec2f force = getForceFromHue();
    //cout<<"force "<<force<<endl;
    //ofDrawLine(0, 0, force.x*demo_scale, force.y*demo_scale);
    
    ofPopMatrix();
    
    //testing
    if (false){
        int box_size = 70;
        for (int c=0; c<BOX_SIZE; c++){
            for (int r=0; r<BOX_SIZE; r++){
                ofFill();
                ofSetColor(box_cols[c][r]);
                ofDrawRectangle(c*box_size, r*box_size, box_size, box_size);
            }
        }
        for (int c=0; c<BOX_SIZE; c++){
            for (int r=0; r<BOX_SIZE; r++){
                ofNoFill();
                ofSetColor(50);
                ofDrawRectangle(c*box_size, r*box_size, box_size, box_size);
                
                string text = "";
                text += "hue:"+ofToString((int)box_cols[c][r].getHue());
                text += "\nmatch:"+ofToString((int) (hue_match[c][r]*100) )+"%";
                text += "\nbri: "+ofToString(box_cols[c][r].getBrightness());
                text += "\nbrfc:"+ofToString((int) (getForceFromBri().y*100) );
                ofSetColor(0);
                if (box_cols[c][r].getBrightness() < 100){
                    ofSetColor(255);
                }
                ofDrawBitmapString(text, c*box_size+5, r*box_size+15);
            }
        }
        
        //target color
        ofColor col;
        col.setHsb(target_hue, target_sat, target_bri);
        ofFill();
        ofSetColor(col);
        ofDrawRectangle(4.5*box_size, 0, box_size, box_size);
        ofNoFill();
        ofSetColor(0);
        ofDrawRectangle(4.5*box_size, 0, box_size, box_size);
    }
}

void Particle::kill(){
    anim_timer = 0;
    is_dead = true;
}

void Particle::save(Goal * goal){
    anim_target_pos.x = goal->box.x + goal->box.width/2;
    anim_target_pos.y = goal->box.y + goal->box.height/2;
    is_saved = true;
    anim_timer = 0;
}

float Particle::getHueMatchPrc(ofColor col){
    float other_hue = col.getHue();
    
    float a = MAX(other_hue,target_hue);
    float b = MIN(other_hue,target_hue);
    
    float dif = MIN( a-b, b-(a-256));
    
    return 1.0 - (dif/255.0);
}

ofVec2f Particle::getForceFromHue(){
    ofVec2f force;
    force.set(0,0);
    
    float max = 0.3;
    
    for (int c=0; c<BOX_SIZE; c++){
        for (int r=0; r<BOX_SIZE; r++){
            ofVec2f dir;
            dir.set( c-2 , r-2);
            dir.normalize();
            
//            cout<<"c: "<<c<<"  r: "<<r<<"  match "<<hue_match[c][r]<<endl;
//            cout<<"   dir "<<dir<<endl;
            dir *= hue_match[c][r] * max;
            
            force += dir;
        }
    }
    
    return force;
}

ofVec2f Particle::getForceFromBri(){
    float bri = box_cols[2][2].getBrightness();
    
    float cutoff = 50;
    ofVec2f max_force = ofVec2f(0,-1);
    ofVec2f force = ofVec2f(0,0);
    
    if (bri < cutoff){
        float prc = 1.0 - bri/cutoff;
        force = max_force * prc;
    }
    
    return force;
    
}

//
//  PowerSwitch.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#include "PowerSwitch.hpp"

void PowerSwitch::setup(float x, float y){
    pos.set(x,y);
    
    hit_range = 60;
    is_active = false;
    can_toggle = true;
    
}
void PowerSwitch::update(ofVec2f player_pos){
    
    float dist = ofDist(player_pos.x, player_pos.y, pos.x, pos.y);
    
    if (dist < hit_range && can_toggle){
        is_active = !is_active;
        can_toggle = false;
    }
    
    if (dist > hit_range+20){
        can_toggle = true;
    }
}

void PowerSwitch::draw(){
//    ofSetColor(255,255,0, 150);
//    ofDrawCircle(pos.x, pos.y, hit_range);
    
    for (int i=0; i<5; i++){
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        
        float triangle_size = hit_range;
        float scale = 1 + (float)i * 0.25;
        ofScale(scale, scale);
        
        if (is_active){
            ofRotateXDeg(ofGetElapsedTimef() * ofMap(i, 0, 5, 10, 50));
            ofRotateZDeg(ofGetElapsedTimef() * ofMap(i, 0, 5, 72, 23));
        }
        
        ofNoFill();
        ofSetLineWidth(2);
        ofSetColor(ofColor::purple);
        ofDrawTriangle(0, -triangle_size/2, triangle_size/2, triangle_size/2, -triangle_size/2, triangle_size/2);
        
        ofPopMatrix();
    }
    
    
    
}

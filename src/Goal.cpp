//
//  Goal.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#include "Goal.hpp"

void Goal::setup(float x, float y, float w, float h){
    box.set(x, y, w, h);
}

void Goal::update(){
    
}

void Goal::draw_fbo(){
    ofSetColor(30,30,255, 100);
    ofNoFill();
    ofSetLineWidth(1);
    float mod = ofGetFrameNum() % 50;
    ofDrawRectangle(box.x-mod/2, box.y-mod/2, box.width+mod, box.height+mod);
    ofDrawRectangle(box.x+mod/2, box.y+mod/2, box.width-mod, box.height-mod);
}

void Goal::draw_top(){
    ofSetColor(30,30,255, 80);
    ofFill();
    ofDrawRectangle(box);
    
    ofSetColor(30,30,255, 200);
    ofNoFill();
    ofSetLineWidth(3);
    ofDrawRectangle(box);
    
}

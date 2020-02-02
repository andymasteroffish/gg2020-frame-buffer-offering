//
//  Player.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#include "Player.hpp"

void Player::setup(float start_x, float start_y){
    pos.set(start_x, start_y);
    
    move_speed = 200;
}
void Player::move(int x_dir, int y_dir){
    
    pos.x += x_dir * move_speed * ofGetLastFrameTime();
    pos.y += y_dir * move_speed * ofGetLastFrameTime();
}

void Player::draw(){
    
    ofPushMatrix();
    
    ofTranslate(pos.x, pos.y);
    
    ofFill();
    ofSetColor(255,50,50);
    ofDrawCircle(0, 0, 50);
    
    ofPopMatrix();
    
}

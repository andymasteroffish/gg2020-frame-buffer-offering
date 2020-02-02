//
//  Button.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/2/20.
//

#include "Button.hpp"

void Button::setup(string _text, int target, int _center_x, int _center_y, int w, int h, ofTrueTypeFont * _font){
    
    center_x = _center_x;
    center_y = _center_y;
    
    target_level = target;
    
    box.set(center_x-w/2, center_y-h/2, w, h);
    font = _font;
    text = _text;
    
    col.set(248, 181, 248);//223, 22, 223);
    
    is_level_select = false;
    mouse_over = false;
    
    can_click = false;
}

void Button::update(){
    float hue = ofGetFrameNum() % 255;// col.getHue();
    //hue += ofGetLastFrameTime() * 10;
//    if (hue > 255){
//        hue -= 255;
//    }
    col.setHue(hue);
}

void Button::draw_fbo(){
    ofFill();
    ofSetColor(col.r, col.g, col.b, mouse_over ? 155 : 75);
    ofDrawRectangle(box);
    
    draw_top();
    
    if (mouse_over){
        int cycles = 4;
        if (can_click)  cycles = 8;
        for (int i=0; i<cycles; i++){
            int max_dist = 50;
            int this_dist = ofGetFrameNum() + (max_dist/cycles) * i;
            this_dist = this_dist % max_dist;
            
            ofNoFill();
            ofSetLineWidth(1);
            ofSetColor(col);
            ofDrawRectangle(box.x-this_dist/2, box.y-this_dist/2, box.width+this_dist, box.height+this_dist);
        }
    }
}

void Button::draw_top(){
    ofSetLineWidth(1);
    
    if (can_click){
        ofFill();
        ofSetColor(col.r, col.g, col.b, 100);
        ofDrawRectangle(box);
    }
    
    ofNoFill();
    ofSetColor(col);
    ofDrawRectangle(box);
    
    ofFill();
    if (!is_level_select){
        font->drawString(text, center_x-font->stringWidth(text)/2, center_y+font->stringHeight(text)/2);
    }else{
        font->drawString(text, box.x+20, box.y+30);
    }
}

void Button::mouseMoved(int x, int y ){
    mouse_over = box.inside(x,y);
    if (!mouse_over){
        can_click = false;
    }
}
void Button::mousePressed(int x, int y){
    can_click = box.inside(x, y);
}
bool Button::mouseReleased(int x, int y){
    bool return_val = can_click;
    can_click = false;
    return return_val;
}

//
//  Slider.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 2/1/20.
//

#include "Slider.hpp"

void Slider::setup(string _name, float pos_x, float pos_y, int size ){
    name = _name;
    top_left.set(pos_x, pos_y);
    box_size = size;
    
    val_x = NULL;
    val_y = NULL;
    
    label_x = "none";
    label_y = "none";
    
    
    min_x = min_y = 0;
    max_x = max_y = box_size;
    default_x = default_y = box_size/2;
    
    default_cursor.set(box_size/2, box_size/2);
    
    set_to_defaults();
    
    can_move = false;
    
    volume_min = 0.1;
    volume_max = 0.75;
    
}

void Slider::set_audio(string name_a, string name_b){
    float audio_speed = 0.3;
    x_clip.load(name_a);
    x_clip.setSpeed(audio_speed);
    x_clip.setLoop(true);
    //x_clip.play();
    
    y_clip.load(name_b);
    y_clip.setSpeed(audio_speed);
    y_clip.setLoop(true);
    //y_clip.play();
    
    update_values();
}

void Slider::set_mute(bool muted){
    if (muted){
        x_clip.stop();
        y_clip.stop();
    }else{
        x_clip.play();
        y_clip.play();
    }
}

void Slider::set_x(string label, float * _val, float min, float max, float default_val){
    label_x = label;
    val_x = _val;
    min_x = min;
    max_x = max;
    default_x = default_val;
    
    cursor.x = ofMap(default_val, min, max, 0, box_size);
    default_cursor.x = cursor.x;
    
    update_values();
}

void Slider::set_y(string label, float * _val, float min, float max, float default_val){
    label_y = label;
    val_y = _val;
    min_y = min;
    max_y = max;
    default_y = default_val;
    
    cursor.y = ofMap(default_val, min, max, 0, box_size);
    default_cursor.y = cursor.y;
    
    update_values();
}

void Slider::set_to_defaults(){
    
//    cursor.x = ofMap(default_x, min_x, max_x, 0, box_size);
//    cursor.y = ofMap(default_y, min_y, max_y, 0, box_size);

    cursor.set(default_cursor);
    update_values();
}

void Slider::mouse_pressed(int mouseX, int mouseY){
    can_move = (mouseX >= top_left.x && mouseX <= top_left.x+box_size && mouseY >= top_left.y && mouseY <= top_left.y+box_size);
}

void Slider::update(int mouseX, int mouseY){
    if (!can_move){
        return;
    }
    
    //make sure we're in bounds
    mouseX = CLAMP(mouseX, top_left.x, top_left.x+box_size);
    mouseY = CLAMP(mouseY, top_left.y, top_left.y+box_size);
    
    cursor.x = mouseX - top_left.x;
    cursor.y = mouseY - top_left.y;
    
    //try to set the values
    update_values();
    
}

void Slider::update_values(){
    if (val_x != NULL){
        *val_x = ofMap(cursor.x, 0, box_size, min_x, max_x);
    }
    if (val_y != NULL){
        *val_y = ofMap(cursor.y, 0, box_size, min_y, max_y);
    }
    
    //change the volumes
    x_clip.setVolume( ofMap(cursor.x, 0, box_size, volume_min, volume_max));
    y_clip.setVolume( ofMap(cursor.y, 0, box_size, volume_min, volume_max));
    //cout<<name<< " x vol "<<x_clip.getVolume()<<endl;
}

void Slider::draw(){
    ofPushMatrix();
    
    ofSetLineWidth(1);
    
    ofTranslate(top_left.x, top_left.y);
    
    //box
    ofSetColor(100);
    ofFill();
    ofDrawRectangle(0, 0, box_size, box_size);
    
    //default values
    ofSetColor(150);
    ofDrawLine(0, default_cursor.y, box_size, default_cursor.y);
    ofDrawLine(default_cursor.x, 0, default_cursor.x, box_size);
    
    //bounding box
    ofSetColor(240,30,20);
    ofNoFill();
    ofDrawRectangle(0, 0, box_size, box_size);
    
    
    //top label
    ofDrawBitmapString(name, 2, -3);
    
    //bottom values
    string bottom_text = label_x;
    if (val_x != NULL){
        bottom_text += ": "+float2string(*val_x);
    }
    bottom_text+="\n"+label_y;
    if (val_y != NULL){
        bottom_text += ": "+float2string(*val_y);
    }
    ofDrawBitmapString(bottom_text, 0, box_size+15);
    
    
    //cursor
    ofNoFill();
    ofDrawCircle(cursor.x, cursor.y, 4);
    
    //some lines
    ofDrawLine(0, cursor.y, box_size, cursor.y);
    ofDrawLine(cursor.x, 0, cursor.x, box_size);
    
    ofPopMatrix();
}

string Slider::float2string(float val){
    int a = val * 100;
    float b = (float)a / 100.0;
    return ofToString(b);
}

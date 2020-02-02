//
//  Game.cpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#include "Game.hpp"

void Game::setup(int _level_num, int screen_w, int screen_h){
    
    level_num = _level_num;
    fbo.allocate(screen_w, screen_h);
    
    board_w = fbo.getWidth() * 2;
    board_h = fbo.getHeight() * 2;
    
    cam_pos.set(0, 0);
    player.setup(0, 0);
    
    holding_up =holding_down = holding_left = holding_right = false;
    
    //debug_draw_over = false;
    
    //default visual settings
    frame_delay_f = 25;
    
    
    
    //FB0 Mix Things
//    gui->fb0blend= 0.5;//mix amount
//    gui->fb0lumakeyvalue= 0.52;// 0.1; //Andy
//    gui->fb0lumakeythresh= 0.52;//0.6;
//    gui->fb0delayamnt= frame_delay_f;// 0;
    
    //effects
    delay_amount_effect = false;
    rotate_effect = false;
    hue_effect = false;
    
    if (level_num == 1){
        PowerSwitch p1;
        p1.setup(-120, 130);
        power_switches.push_back(p1);
        
        PowerSwitch p2;
        p2.setup(-250, -100);
        power_switches.push_back(p2);
    }
    
}

void Game::keyPressed(int key){
    if (key == OF_KEY_UP)   holding_up = true;
    if (key == OF_KEY_DOWN)   holding_down = true;
    if (key == OF_KEY_LEFT)   holding_left = true;
    if (key == OF_KEY_RIGHT)   holding_right = true;
//    
//    if (key == 'd'){
//        debug_draw_over = !debug_draw_over;
//    }
    
//    //trying effects
//    if (key == '0')     delay_amount_effect = !delay_amount_effect;
//    if (key == '1')     rotate_effect = !rotate_effect;
//    if (key == '2')     hue_effect = !hue_effect;
}

void Game::keyReleased(int key){
    
    if (key == OF_KEY_UP)   holding_up = false;
    if (key == OF_KEY_DOWN)   holding_down = false;
    if (key == OF_KEY_LEFT)   holding_left = false;
    if (key == OF_KEY_RIGHT)   holding_right = false;
}

void Game::update(){
    float cam_follow_range = 100;
    if ( abs(player.pos.x -cam_pos.x) > cam_follow_range || abs(player.pos.y-cam_pos.y) > cam_follow_range ){
        float zeno = 0.05;
        cam_pos = (1.0-zeno) * cam_pos + zeno * player.pos;
    }
    
    if (holding_up)     player.move(0,-1);
    if (holding_down)     player.move(0,1);
    if (holding_left)     player.move(-1,0);
    if (holding_right)     player.move(1,0);
    
    //stay in range
    player.pos.x = CLAMP(player.pos.x, -board_w/2, board_w/2);
    player.pos.y = CLAMP(player.pos.y, -board_h/2, board_h/2);
    
    //chekc power switches
    for (int i=0; i<power_switches.size(); i++){
        power_switches[i].update(player.pos);
    }
    
    //apply effects
    float effect_zeno = 0.03;
    
    //check the switches
//    delay_amount_effect = power_switches[0].is_active;
//    rotate_effect = power_switches[1].is_active;
//    hue_effect = power_switches[2].is_active;
    
    //delay amount
//    float target_delay_amount = 25;
//    if (delay_amount_effect){
//        target_delay_amount = 1;
//    }
//    zeno(frame_delay_f, target_delay_amount, effect_zeno);
//    gui->fb0delayamnt = frame_delay_f;
//
//    //rotate effect
//    float target_cam1_scale = 1;
//    float target_fb0_rotate = 0;
//    if (rotate_effect){
//        target_cam1_scale = 1.51;
//        target_fb0_rotate = -56;
//    }
//    gui->cam1_scale = (1.0-effect_zeno)*gui->cam1_scale + effect_zeno*target_cam1_scale;
//    gui->fb0_rotate = (1.0-effect_zeno)*gui->fb0_rotate + effect_zeno*target_fb0_rotate;
//
//    //hue effect
//    float target_huex = 10;
//    float target_huex_mod = 10;
//    float target_huex_offset = 0;
//    float target_huex_lfo = 0;
//    if (hue_effect){
//        target_huex = 15;
//        target_huex_mod = 13;
//        target_huex_offset = 7;
//        target_huex_lfo = 15;
//    }
//    zeno(gui->fb0_hue, target_huex, effect_zeno);
//    zeno(gui->fb0_huex_mod, target_huex_mod, effect_zeno);
//    zeno(gui->fb0_huex_offset, target_huex_offset, effect_zeno);
//    zeno(gui->fb0_huex_lfo, target_huex_lfo, effect_zeno);
    
}

//this should happend inside fbo.begin() and fbo.end() in ofApp
void Game::draw(){
    
    
    
    //background
    ofFill();
    ofSetColor(255);
    ofDrawRectangle(0, 0, fbo.getWidth(), fbo.getHeight());
    
    //cout<<"size "<<fbo.getWidth()<<","<<fbo.getHeight()<<endl;
    
    //game stuff
    ofPushMatrix();
    ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
    ofTranslate(-cam_pos.x, -cam_pos.y);
    
    //board
    int tile_size = 40;
    int cols = board_w/tile_size;
    int rows = board_h/tile_size;
    
    ofColor pink, green, salmon;
    pink.set(255, 20, 232);
    green.set(30, 112, 46);
    salmon.set(169, 45, 45);//113, 30, 30);
    
    
    
    
    ofSetColor(green);
    ofFill();
    ofDrawRectangle(-board_w/2, -board_h/2, board_w, board_h);
    
    if (level_num == 1 || level_num == 2){
        for (int x=0; x<cols; x++){
            for (int y=0; y<rows; y++){
                float tile_x = -board_w/2 + x*tile_size;
                float tile_y = -board_h/2 + y*tile_size;
                
                if ( (x+y) % 2 ==0){
                    ofSetColor(pink);
                    ofPushMatrix();
                    
                    
                    if (level_num == 1){
                        ofTranslate(tile_x, tile_y);
                        ofRotateRad(ofGetElapsedTimef());
                    }
                    if (level_num == 2){
                        ofTranslate(tile_x, tile_y);
                        float scale = 1 + sin(ofGetElapsedTimef()) * 0.1;
                        if ( (x+y) % 4 ==0 ){
                            scale = 1 - (scale-1);
                        }
                        ofRotateRad(PI/5);
                        ofScale(scale, scale);
                    }
                    
                    ofDrawRectangle(-tile_size/2, -tile_size/2, tile_size, tile_size);
                    ofPopMatrix();
                }
            }
        }
    }
    
    
    
    //power switches
//    for (int i=0; i<power_switches.size(); i++){
//        power_switches[i].draw();
//    }
    
    //player
    //player.draw();
    
    ofPopMatrix();
    
    //black_bar
    if (level_num == 2){
        ofSetColor(0,0,10, 200+ofGetFrameNum() % 100);
        ofFill();
        ofDrawRectangle(30, fbo.getHeight()*0.8, fbo.getWidth()-60, 100);
    }
    
    //bg
    if (level_num == 3 || level_num == 4){
        ofSetColor(pink);
        ofFill();
        ofDrawRectangle(0,0, fbo.getWidth(), fbo.getHeight());
        
        int darken = 15;
        ofSetColor(pink.r-darken, pink.g-darken, pink.b-darken);
        int spacing = 50;
        int bg_cols = fbo.getWidth() / spacing ;
        int bg_rows = fbo.getHeight() / spacing ;
        float angle_step = TWO_PI/3;
        for (int c = -1; c<=bg_cols; c++){
            for (int r = -1; r<=bg_rows; r++){
                
                
                if ( (c+r)%2 == 0){
                    ofPushMatrix();
                    ofTranslate(c*spacing, r*spacing);
                    
                    float dist = 15;
                    
                    ofFill();
                    ofBeginShape();
                    for (int i=0; i<3; i++){
                        float angle = ofGetElapsedTimef()*0.2 + i*angle_step;
                        ofVertex(cos(angle)*dist, sin(angle)*dist);
                    }
                    ofEndShape(true);
                    
                    ofPopMatrix();
                }
                
            }
        }
    }
    
    //level 3 - rotation intro
    if (level_num == 3){
        
        ofSetColor(green);
        int count = 0;
        for (int y=-100; y<=fbo.getWidth(); y+= 50){
            count++;
            float box_w = 60;
            float box_h = 120;
            float padding = 76;
            ofPushMatrix();
            ofTranslate(padding, y);
            float rotation = sin( ofGetElapsedTimef()+ (count * PI/4)) * 0.1 ;
            ofRotateRad(rotation);
            //ofRotateDeg(count * 45 + ofGetElapsedTimef() * 10);
            ofDrawRectangle(-box_w/2, -box_h/2, box_w, box_h);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(fbo.getWidth()-padding, y);
            ofRotateRad(rotation);
            ofDrawRectangle(-box_w/2, -box_h/2, box_w, box_h);
            ofPopMatrix();
            
            ofPushMatrix();
            ofTranslate(y, padding);
            ofRotateRad(rotation + PI/2);
            ofDrawRectangle(-box_w/2, -box_h/2, box_w, box_h);
            ofPopMatrix();
        }
        
        //ofDrawRectangle(50,0,fbo.getWidth()-100,50);
    }
    
    //level 4 - rotation and zoom hard
    if (level_num == 4){
        
        
        ofSetColor(green);
        ofFill();
        ofDrawRectangle(0, 10, fbo.getWidth(), 30);
        ofDrawRectangle(0, 50, fbo.getWidth(), 20);
        
        ofDrawRectangle(fbo.getWidth()-50,0,40,fbo.getHeight());
        
        //ofDrawRectangle(100, fbo.getHeight()-100, 50, 50);
        
        //ofSetColor(salmon);
        ofPushMatrix();
        ofTranslate(fbo.getWidth()*0.75, fbo.getHeight()*0.75);
        ofRotateDeg(-30);
        ofDrawRectangle(-100, -20, 200, 40);
        ofPopMatrix();
    }
    
    
    
    
    
    //debug info
//    ofSetColor(0);
//    string debug_text = "";
//    debug_text += "x: "+ofToString(player.pos.x)+"\ny: "+ofToString(player.pos.y);
//    ofDrawBitmapString(debug_text, 10, 15);
    
    
}

void Game::zeno(float &val, float target, float prc){
    val = (1.0-prc)*val + prc*target;
}

//void Game::zeno(int &val, float target, float prc){
//    val = (1.0-prc)*((float)val) + prc*target;
//}






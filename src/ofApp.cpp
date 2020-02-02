/*
 re the midi biz
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */



#include "ofApp.h"


#include <iostream>





//--------------------------------------------------------------
void ofApp::setup() {
    
    mute_sound = false;
    
    save_file_name = "scores.txt";
    
    //ofBackground(69, 47, 79);
    ofBackground(0);
    ofSetVerticalSync(true);
    ofSetEscapeQuitsApp(false);
    
    ofSetFrameRate(30);
   // ofDisableAlphaBlending();
    //ofSetVerticalSync(FALSE);
    
    game_w = 640;
    game_h = 480;
    
    title_font.load("ChevyRay - Rise.ttf", 50, true, true, true);
    big_font.load("ChevyRay - Chocolate Mono", 30, true, true, true);
    medium_font.load("ChevyRay - Chocolate Mono", 22, true, true, true);
    small_font.load("ChevyRay - Chocolate Mono", 14, true, true, true);
    tiny_font.load("ChevyRay - Chocolate Mono", 11, true, true, true);
    
    win_sound.load("sound/wizard_harmonica.wav");
    win_sound.setSpeed(0.3);
    lose_sound.load("sound/big_brother_mayhem.wav");
    lose_sound.setMultiPlay(true);
    lose_sound.setSpeed(3);
    
    for (int i=0; i<6; i++){
        ofSoundPlayer menu_sound;
        menu_sound.load("sound/i_shall_be_released.wav");
        menu_sounds.push_back(menu_sound);
        //menu_sound.setMultiPlay(true);
    }
    
    level_names[0] = "To the Right";
    level_names[1] =  "Black is Up";
    level_names[2] =  "Spin Land";
    level_names[3] =  "Rabit Hole";
    
    for (int i=0; i<NUM_LEVELS; i++){
        best_times[i] = -1;
    }
    
    //try to laod scores
    ofBuffer buffer = ofBufferFromFile(save_file_name);
    if (buffer.size()){
        int count = 0;
        for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
            string line = *it;
            cout << "score "<<count<<": "<< line << endl;
            if (count < NUM_LEVELS){
                best_times[count] = ofToFloat(line);
            }
            count++;
        }
    }else{
        cout<<"could not find scores file"<<endl;
    }
    
    reset(STATE_TITLE);
    //reset(1);
    
    test_anim_fbo.allocate(game_w, game_h);
    
    debug_draw_test = false;
   
}

//--------------------------------------------------------------
void ofApp::reset(int _level_num){
    
    //reset the menu stuff if we were in a real level
    if (level_num >=0){
        menu_sound_timer = 0;
        cur_menu_sound_speed = 0.75;
        menu_sound_count = 0;
        //menu_sound.setSpeed(cur_menu_sound_speed);
    }
    
    
    level_num = _level_num;
    
    cout<<"level "<<level_num<<endl;
    
    game_state = STATE_PLAYING;
    if (level_num == STATE_TITLE){
        game_state = STATE_TITLE;
    }
    if (level_num == STATE_LEVEL_SELECT){
        game_state = STATE_LEVEL_SELECT;
    }
    
    
    
    int screen_w = game_w;
    int screen_h = game_h;
    screen_offset_y = 35;
    
    if (level_num < 0){
        screen_w = ofGetWidth();
        screen_h = ofGetHeight();
        screen_offset_y = 0;
    }
    
    video_waves.gui = gui;
    video_waves.setup(screen_w, screen_h);
    
    game.gui = gui;
    game.setup(level_num, screen_w, screen_h);
    
    //setting defaults
    gui->blur_amount = 0;
    gui->FBmix= 0; //0;      //Andy
    gui->FB1mix=0;  //set this to 2 after a few frames
    
    gui->fb0blend= 0.5;//mix amount
    gui->fb1blend= 1;//mix amount
    gui->fb0lumakeyvalue= 0.52;// 0.1; //Andy
    gui->fb0lumakeythresh= 0.52;//0.6;
    gui->fb0delayamnt= 25;// 0;
    gui->fb1delayamnt= 31;// 0;
    
    gui->fb0blend = 0.45;
    gui->fb1blend = 0.58;
    
    gui->fb1_x_displace = 0;
    gui->fb1_y_displace = 0;
    
    gui->fb1_rotate = 0;
    gui->fb1_z_displace = 100;
    
    gui->fb1_huex_offset = 0;
    gui->fb1_saturation=10.0;
    gui->fb1_bright=10.0;
    gui->fb1_huex_lfo = 0;
    
    ps.setup(game_w, game_h, &goal, level_num);
    tint_time = 0.5;
    saved_tint_timer = 0;
    dead_tint_timer = 0;
    
    time_in_level = 0;
    
    num_to_save = 20;
    
    //goal
    if (level_num == 1){
        goal.setup(440, 10, 70, 460);
    }
    if (level_num == 2){
        goal.setup(game_w-100, game_h-160, 70, 100);
    }
    if (level_num == 3){
        goal.setup(game_w/2-50, game_h/2-50, 100, 100);
        gui->fb1lumakeyvalue = 0.3;
        gui->fb1lumakeythresh = 0.07;
    }
    if (level_num == 4){
        goal.setup(50, 40, 200, 150);
    }
    
    if (level_num < 0){
        goal.setup(999,999,0,0);
    }
    
    
    //setting up the sliders
    sliders.clear();
    int sliderY = game_h+55;
    int slider_box_size = 170;
    int slider_x_spacing = (ofGetWidth()-slider_box_size*3)/4;
    if (game_state == STATE_PLAYING){
        if (level_num == 1 || level_num == 2){
            Slider slider;
            slider.setup("FB1 Displace", slider_x_spacing, sliderY, slider_box_size);
            slider.set_x("x displace", &gui->fb1_x_displace, -30, 30, 0);
            slider.set_y("y displace", &gui->fb1_y_displace, -30, 30, 0);
            sliders.push_back(slider);
        }
        if (level_num == 3 || level_num ==4){
            Slider slider;
            slider.setup("Rotation", slider_x_spacing, sliderY, slider_box_size);
            slider.set_x("rotate", &gui->fb1_rotate, -100, 100, 0);
            slider.set_y("z displace", &gui->fb1_z_displace, 0, 200, 100);
            sliders.push_back(slider);
            
            //gui->fb1_x_displace = -5;
        }
        
        if (level_num != 3){
            Slider slider;
            slider.setup("Luma Key", slider_x_spacing + 1*(slider_x_spacing+slider_box_size), sliderY, slider_box_size);
            float min = 0;
            if (level_num == 2) min = 0.15;
            slider.set_x("value", &gui->fb1lumakeyvalue, min, 1, min+0.1);
            slider.set_y("threshold", &gui->fb1lumakeythresh, 0, 1, 0.6);
            sliders.push_back(slider);
        }
        
        if (level_num > 1 && level_num != 3){
            Slider slider;
            slider.setup("Mixers", slider_x_spacing + 2*(slider_x_spacing+slider_box_size), sliderY, slider_box_size);
            float start_x = 0.75;
            float start_y = 0.1;
            if (level_num == 4){
                start_x = -1.38;
                start_y = 0.75;
            }
            slider.set_x("FB0 Mix Amount", &gui->fb0blend, -2, 2, start_x);
            slider.set_y("FB1 Mix Amount", &gui->fb1blend, -2, 2, start_y);
            sliders.push_back(slider);
        }
    }
    
    //level names
    if (level_num >= 1 && level_num<=4){
        level_name = level_names[level_num-1];
    }
//    if (level_num == 1) level_name = "To the Right";
//    if (level_num == 2) level_name = "Black = Up";
//    if (level_num == 3) level_name = "Spin Land";
//    if (level_num == 4) level_name = "Rabit Hole";
    
    //buttons
    buttons.clear();
    int button_w = 140;
    int button_h = 50;
    if (game_state == STATE_TITLE){
        Button b;
        b.setup("PLAY", STATE_LEVEL_SELECT,  game_w/2, ofGetHeight()*0.8, button_w, button_h, &small_font);
        buttons.push_back(b);
    }
    
    //level select buttons
    if (game_state == STATE_LEVEL_SELECT){
        button_w = 250;
        button_h = 60;
        
        for (int i=0; i<NUM_LEVELS; i++){
            Button b;
            string text = level_names[i];
            if (best_times[i] < 0){
                text+="\nno high score";
            }else{
                text+="\nbest time: "+ofToString((int)best_times[i]);
            }
            int start_y = ofGetHeight()*0.4;
            int y_padding = button_h + 30;
            b.setup(text, i+1,  game_w/2, start_y + i*y_padding, button_w, button_h, &small_font);
            b.is_level_select = true;
            buttons.push_back(b);
        }
        
        //menu
        Button b;
        b.setup("Title", STATE_TITLE,  10+60, ofGetHeight()-10-20, 120, 40, &small_font);
        buttons.push_back(b);
    }
    
    //set the sound
    if (level_num == 1){
        sliders[0].set_audio("sound/electric_funeral_atomic_tide.wav", "sound/bloody_sabbath_execution.wav");
        sliders[1].set_audio("sound/angel_duster_end.wav", "sound/angel_duster_power.wav");
    }
    
    if (level_num == 2){
        sliders[0].set_audio("sound/kid_charlemange_solo.wav","sound/naive_beat.wav");
        sliders[1].set_audio("sound/memories_cant_wait.wav","sound/naive_plenty_of_time.wav");
        sliders[2].set_audio("sound/booming_and_zooming_see_me.wav","sound/booming_and_zooming.wav");
    }
    
    if (level_num == 3){
        sliders[0].set_audio("sound/interstellar_overdrive_intro.wav","sound/heart_of_the_sun_intro.wav");
        //sliders[0].set_audio("sound/gypsy_town_wolves.wav", "sound/live_for_toady_countdown.wav");
        sliders[0].volume_max = 1;
    }
    
    if (level_num == 4){
        sliders[0].set_audio("sound/whats_a_girl_to_do_desire.wav","sound/black_swan_ticket.wav");
        sliders[1].set_audio("sound/whats_a_girl_to_do_thrill.wav","sound/the_clock_beat.wav");
        sliders[2].set_audio("sound/81_nothing_more.wav","sound/mouthful_of_diamonds_beat.wav");
    }
    
    for (int i=0; i<sliders.size(); i++){
        sliders[i].set_mute(mute_sound);
    }
    
}


//--------------------------------------------------------------
void ofApp::update() {
    if(game_state == STATE_PLAYING || game_state < 0){
        time_in_level += ofGetLastFrameTime();
    }
    
    //the screen flashes if we set the second buffer to be KEY right away
    if (time_in_level > 2){
        gui->FBmix=2;
        gui->FB1mix=2;
    }
    
    game.update();
    
    for (int i=0; i<buttons.size(); i++){
        buttons[i].update();
    }
    
    //drawing into the game FBO
    game.fbo.begin();
    game.draw();
    
    if (level_num >= 0){
        //the level name
        ofSetColor(50);
        small_font.drawString(ofToString(level_num)+": " +level_name, 10, 30);
        
        //the goal
        goal.draw_fbo();
        
        //drawing game over messages
        if (game_state == STATE_WIN){
            draw_win_message();
        }
        if (game_state == STATE_LOSE){
            draw_lose_message();
        }
    }
    
    if (game_state == STATE_TITLE){
        draw_title(true);
    }
    if (game_state == STATE_LEVEL_SELECT){
        draw_level_select(true);
    }
    
    for (int i=0; i<buttons.size(); i++){
        buttons[i].draw_fbo();
    }
    
    game.fbo.end();
    
    //DONE DRAWING INTO GAME BUFFER
    
    
    video_waves.update();
    
    //andy doing his test thing
    test_anim_fbo.begin();
    ofClear(255);
    
    //background
    ofSetColor(255);
    ofDrawRectangle(0, 0, test_anim_fbo.getWidth(), test_anim_fbo.getHeight());
    
    //color bars
    int num_bars = 6;
    float bar_w = test_anim_fbo.getWidth() /  (num_bars/2);
    
    ofFill();
    for (int i=0; i<num_bars; i++){
        ofSetColor(ofColor::darkGreen);
        if (i%2==0) ofSetColor(255, 117, 241);
        
        int move_val = (int)(ofGetElapsedTimef()*40) ;
        move_val = ofGetMouseX() / 2;
        int x = ( (int)(bar_w*i) + move_val);
        x = x % ((int)test_anim_fbo.getWidth()*2)  - bar_w;
        ofDrawRectangle(x, 0, bar_w, test_anim_fbo.getHeight());
    }
    
    //horizontal bar
    //ofSetColor(100,80,210);
    ofSetColor(0,0,10);
    ofDrawRectangle(0, ofGetMouseY()/2 + 100, test_anim_fbo.getWidth(), 20);
    
    test_anim_fbo.end();
    
    
    //update the particles
    if (debug_draw_test){
        ps.update(&test_anim_fbo);
    }else{
        ps.update(&video_waves.fbo_draw);
    }
    
    //and the goal if relevant
    goal.update();
    
    //check if we won or lost
    if (game_state == STATE_PLAYING){
        
        if (ps.saved_particles >= num_to_save){
            win_round();
            
        }
        if (ps.saved_particles+ps.particles.size() < num_to_save){
            lose_round();
            
        }
    }
    
    //fucking with settings on win/lose
    if (game_state == STATE_WIN){
        gui->fb1_huex_offset += ofGetLastFrameTime() * 0.3;
        game.zeno(gui->fb1_saturation, 14.5, 0.1);
        game.zeno(gui->fb1_bright, 9.5, 0.02);
        game.zeno(gui->fb1_rotate, 3, 0.1);
        game.zeno(gui->fb1_x_displace, 0, 0.01);
        game.zeno(gui->fb1_y_displace, 0, 0.01);
    }
    if (game_state == STATE_LOSE){
        game.zeno(gui->blur_amount, 4, 0.01);
        game.zeno(gui->fb1_y_displace, 20, 0.001);
        game.zeno(gui->fb1_huex_lfo, -12, 0.002);
        if (gui->fb0delayamnt > 1) gui->fb0delayamnt--;
        if (gui->fb1delayamnt > 1) gui->fb1delayamnt--;
    }
    
    if ( (game_state == STATE_TITLE || game_state == STATE_LEVEL_SELECT) && gui->FBmix==2){
        game.zeno(gui->fb1_z_displace, 90, 0.01);
        gui->fb1_huex_lfo = sin(ofGetElapsedTimef()) * 10;
        if (gui->fb1_rotate > -2)   gui->fb1_rotate -= ofGetLastFrameTime() * 0.5;
        if (time_in_level > 6)  game.zeno(gui->fb1blend, -0.3, 0.001);
    }
    
    //dealing with audio
    if (game_state == STATE_WIN){
        win_sound.setVolume(1);
    }else{
        float zeno = 0.02;
        win_sound.setVolume( win_sound.getVolume() * (1.0-zeno));
    }
    if (game_state == STATE_LOSE){
        if (lose_sound.getVolume() > 0){
            lose_sound.setVolume(lose_sound.getVolume() - ofGetLastFrameTime()*0.2);
            
            if (ofRandomuf() < 0.2 && !mute_sound){
                lose_sound.setSpeed( ofRandomuf() < 0.5 ? ofRandom(0.1,0.5) : ofRandom(2,4));
                lose_sound.play();
            }
        }
    }else{
        float zeno = 0.02;
        lose_sound.setVolume( lose_sound.getVolume() * (1.0-zeno));
    }
    
    //menu music
    if (level_num < 0 && !mute_sound){
        
        menu_sound_timer -= ofGetLastFrameTime();
        if (menu_sound_timer < 0){
            menu_sound_count++;
            cur_menu_sound_speed *= 0.9;
            cur_menu_sound_speed = MAX(cur_menu_sound_speed, 0.15);
            int id = menu_sound_count%menu_sounds.size();
            
            menu_sound_timer += ofMap(cur_menu_sound_speed, 0.675, 0.15, 2, 4);
            
            menu_sounds[id].play();
            menu_sounds[id].setSpeed(cur_menu_sound_speed);
            menu_sounds[id].setVolume(0.75);
            //cout<<"menu music speed "<<cur_menu_sound_speed<<endl;
        }
    }else{
        for (int i=0; i<menu_sounds.size(); i++){
            menu_sounds[i].setVolume(menu_sounds[i].getVolume() * 0.95);
        }
    }
    
    //tints
    dead_tint_timer -= ofGetLastFrameTime();
    saved_tint_timer -= ofGetLastFrameTime();
    
    if (ps.saved_flag){
        ps.saved_flag = false;
        saved_tint_timer = tint_time;
    }
    if (ps.dead_flag){
        ps.dead_flag = false;
        dead_tint_timer = tint_time;
    }
    
}

//--------------------------------------------------------------
void ofApp::win_round(){
    game_state = STATE_WIN;
    
    if(!mute_sound) win_sound.play();
    
    int button_w = 190;
    int button_h = 50;
    
    int select_x = game_w*0.66;
    
    if (level_num < NUM_LEVELS){
        Button b;
        b.setup("NEXT LEVEL", level_num+1,  game_w*0.33, game_h*0.75, button_w, button_h, &small_font);
        buttons.push_back(b);
    }else{
        select_x = game_w/2;
    }
    
    Button b;
    b.setup("LEVEL SELECT", STATE_LEVEL_SELECT,  select_x, game_h*0.75, button_w, button_h, &small_font);
    buttons.push_back(b);
    
    //new high score?
    if (time_in_level < best_times[level_num-1] || best_times[level_num-1] < 0){
        best_times[level_num-1] = time_in_level;
        
        //save the scores
        ofFile myTextFile;
        myTextFile.open(save_file_name,ofFile::WriteOnly);
        for (int i=0; i<NUM_LEVELS; i++){
            myTextFile<<ofToString(best_times[i])<<endl;
        }
    }
}
//--------------------------------------------------------------
void ofApp::lose_round(){
    game_state = STATE_LOSE;
    if(!mute_sound) lose_sound.play();
    lose_sound.setVolume(1);
    
    int button_w = 190;
    int button_h = 50;
    
    Button b;
    b.setup("RETRY", level_num,  game_w*0.33, game_h*0.75, button_w, button_h, &small_font);
    buttons.push_back(b);
    
    
    Button b2;
    b2.setup("LEVEL SELECT", STATE_LEVEL_SELECT,  game_w*0.66, game_h*0.75, button_w, button_h, &small_font);
    buttons.push_back(b2);
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    ofPushMatrix();
    
    ofTranslate(0, screen_offset_y);
    
    video_waves.draw(&game.fbo);
    
    //goal
    if (game_state == STATE_PLAYING){
        goal.draw_top();
    }
    
    //particles
    ps.draw();
    
    //drawing game over messages
    if (game_state == STATE_WIN){
        draw_win_message();
    }
    if (game_state == STATE_LOSE){
        draw_lose_message();
    }
    
    if (debug_draw_test){
        ofSetColor(255);
        test_anim_fbo.draw(0,0);
        //test_anim_fbo.readToPixels(pix);
    }
    
    if (game_state == STATE_TITLE){
        draw_title(false);
    }
    if (game_state == STATE_LEVEL_SELECT){
        draw_level_select(false);
    }
    
    for (int i=0; i<buttons.size(); i++){
        buttons[i].draw_top();
    }
    
    
    ofPopMatrix();
    
    
    //sliders
    for (int i=0; i<sliders.size(); i++){
        sliders[i].draw();
    }
    
    //info fo level 1
    if (level_num == 1){
        float col_range = ofMap(time_in_level,0,10,90,0);
        float col_val = 255-col_range + sin(ofGetElapsedTimef()*4) * col_range;
        //cout<<"col "<<col_val<<endl;
        ofSetColor(col_val);
        //ofSetColor(200 + ((int)(ofGetFrameNum()*1))%55 );
        string text = "Use your mouse to\nmanipulate these\npanels\n\npush the yellow\nparticles to\nthe blue goal\n\nSave 20 to win";
        tiny_font.drawString(text, sliders[1].top_left.x+ sliders[1].box_size+20, sliders[1].top_left.y+15);
    }
    
    //stats on the top
    if (level_num >= 0){
        ofColor info_col(255,255,50);
        ofSetColor(info_col);
        int controls_y = 15;
        ofDrawBitmapString("R to reset", 10, controls_y);
        ofDrawBitmapString("ESC for menu", 200, controls_y);
        
        string on_text = (mute_sound ? "off" : "on");
        string mute_text = "sound: " + on_text + " (m to toggle)";
        ofDrawBitmapString(mute_text, 440, controls_y);
        
        int info_y = 30;
        ofDrawBitmapString("particles: "+ofToString(ps.particles.size()), 10, info_y);
        
        ofDrawBitmapString("time: "+ofToString( (int)time_in_level), 440, info_y);
        
        ofColor red(255,0,0);
        ofColor green(0,255,0);
        ofSetColor(info_col);
        if (saved_tint_timer > 0){
            float prc = saved_tint_timer/tint_time;
            ofColor this_col;
            this_col = (1.0-prc)*info_col + prc*green;
            ofSetColor(this_col);
        }
        ofDrawBitmapString("saved: "+ofToString(ps.saved_particles)+"/"+ofToString(num_to_save), 200, info_y);
        
        ofSetColor(info_col);
        if (dead_tint_timer > 0){
            float prc = dead_tint_timer/tint_time;
            ofColor this_col;
            this_col = (1.0-prc)*info_col + prc*red;
            ofSetColor(this_col);
        }
        ofDrawBitmapString("dead: "+ofToString(ps.dead_particles), 320, info_y);
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::draw_win_message(){
    string winner_message = "LEVEL COMPLETE";
    ofColor col;
    col.set(130, 203, 237);
    col.setHue( (ofGetFrameNum()*3) % 256);
    ofSetColor(col);
    ofVec2f winner_text_anchor;
    winner_text_anchor.y = 150;
    winner_text_anchor.x = game_w/2 - big_font.stringWidth(winner_message)/2;
    
    big_font.drawString(winner_message, winner_text_anchor.x, winner_text_anchor.y);
    
    string time_message = "time: "+sliders[0].float2string(time_in_level);
    medium_font.drawString(time_message, game_w/2-medium_font.stringWidth(time_message)/2, winner_text_anchor.y+100);
    
}

//--------------------------------------------------------------
void ofApp::draw_lose_message(){
    string message = "YOU LOSE";
    
    for (int i=0; i<3; i++){
        ofColor col;
        col.set(223, 22, 22);
        col.setBrightness( (ofGetFrameNum()*4 + i*37) % 256);
        ofSetColor(col);
        ofVec2f anchor;
        anchor.y = 150 + i*50;
        anchor.x = game_w/2 - big_font.stringWidth(message)/2;
        
        anchor.x -= ofRandom(10);
        
        big_font.drawString(message, anchor.x, anchor.y);
    }
    
//    string reset_message = "R to restart";
//    ofColor col2;
//    col2.set(223, 22, 22);
//    col2.setSaturation( (ofGetFrameNum()*4 ) % 256);
//    ofSetColor(col2);
//    medium_font.drawString(reset_message, game_w/2-medium_font.stringWidth(reset_message)/2, game_h * 0.7);
}


//--------------------------------------------------------------
void ofApp::draw_title(bool in_fbo){
    
    if (in_fbo){
        ofSetColor(0);
        ofFill();
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if (in_fbo){
        
        float fade_prc = time_in_level / 2.0;
        
        ofSetColor(223, 22, 223, 255 * fade_prc);
        ofFill();
        draw_logo(ofGetWidth()/2, ofGetHeight()/2, false);
    }
    ofSetColor(255);
    ofNoFill();
    draw_logo(ofGetWidth()/2, ofGetHeight()/2, true);
    
    //credits
    ofSetColor(200);
    ofNoFill();
    int padding = 20;
    small_font.drawStringAsShapes("game by Andy Wallace - @andy_makes", padding, 30);
    //small_font.drawStringAsShapes("@andy_makes", ofGetWidth()-padding-small_font.stringWidth("@andy_makes"), 30);
    small_font.drawStringAsShapes("using Video_Waaaves by Andrei Jay", padding, 60);
    //small_font.drawStringAsShapes("Andrei Jay", ofGetWidth()-padding-small_font.stringWidth("Andrei Jay"), 60);
    small_font.drawStringAsShapes("made at LIU Post for GGJ20", padding, 90);
    
}
//--------------------------------------------------------------
void ofApp::draw_logo(int x, int y, bool outline){
    ofPushMatrix();
    ofTranslate(x,y);
    draw_centered_string(&title_font, "FRAME", -game_w*0.1, -100, outline);
    draw_centered_string(&title_font, "BUFFER", game_w*0.0, 0, outline);
    draw_centered_string(&title_font, "OFFERING", game_w*0.1, 100, outline);
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::draw_level_select(bool in_fbo){
    if (in_fbo){
        ofSetColor(0);
        ofFill();
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    if (in_fbo){
        ofSetColor(111, 208, 241);
    }else{
        ofSetColor(255);
    }
    draw_centered_string(&big_font, "LEVEL SELECT", game_w/2, ofGetHeight()*0.25, !in_fbo);
}

//--------------------------------------------------------------
void ofApp::draw_centered_string(ofTrueTypeFont * font, string text, int x, int y, bool outline){
    if (!outline){
        font->drawString(text, x-font->stringWidth(text)/2, y);
    }
    else{
        font->drawStringAsShapes(text, x-font->stringWidth(text)/2, y);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    
    
    if (key == 'd'){
        for (int i=0; i<sliders.size(); i++){
            sliders[i].set_to_defaults();
        }
    }
    
    if (key == 'm') toggle_mute();
    
    if (key == 27)  reset(STATE_TITLE);     //escape
    if (key == 'r') reset(level_num);
    if (key == '1') reset(1);
    if (key == '2') reset(2);
    if (key == '3') reset(3);
    if (key == '4') reset(4);
    
    game.keyPressed(key);
    
    
    //debug
    if (false){
        if (key == 't'){
            debug_draw_test = !debug_draw_test;
        }
        if (key == 'w'){
            for (int i=0; i<ps.particles.size(); i++){
            //for (int i=0; i<1; i++){
                ps.particles[i].pos.x = goal.box.x + 5;
                ps.particles[i].pos.y = goal.box.y + 5;
            }
        }
        if (key == 'l'){
            for (int i=0; i<ps.particles.size(); i++){
            // for (int i=0; i<1; i++){
                ps.particles[i].pos.x = ofGetWidth()/2;
                ps.particles[i].pos.y = -1;
            }
        }
    }

}

//--------------------------------------------------------------
void ofApp::toggle_mute(){
    mute_sound = !mute_sound;
    for (int i=0; i<sliders.size(); i++){
        sliders[i].set_mute(mute_sound);
    }
}

//--------------------------------------------------------------
void ofApp::exit() {
	
	
}



//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    game.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    for (int i=0; i<buttons.size(); i++){
        buttons[i].mouseMoved(x, y-screen_offset_y);
    }
}


//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    for (int i=0; i<sliders.size(); i++){
        sliders[i].update(x,y);
    }
    for (int i=0; i<buttons.size(); i++){
        buttons[i].mouseMoved(x, y-screen_offset_y);
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    for (int i=0; i<sliders.size(); i++){
        sliders[i].mouse_pressed(x,y);
    }
    
    for (int i=0; i<buttons.size(); i++){
        buttons[i].mousePressed(x, y-screen_offset_y);
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    for (int i=0; i<buttons.size(); i++){
        if(buttons[i].mouseReleased(x, y-screen_offset_y)){
            cout<<"you clicked "<<buttons[i].text<<endl;
            reset(buttons[i].target_level);
        }
    }
}

/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#pragma once

#include "ofMain.h"


#include "VideoWaves.hpp"

#include "Game.hpp"
#include "ParticleSystem.hpp"
#include "Slider.hpp"
#include "Goal.hpp"
#include "Button.hpp"

class ofApp : public ofBaseApp {
	
public:
	
	void setup();
    void reset(int _level_num);
	void update();
    
    void win_round();
    void lose_round();
    
	void draw();
    
    void draw_win_message();
    void draw_lose_message();
    
    void draw_title(bool in_fbo);
    void draw_logo(int x, int y, bool outline);
    
    void draw_level_select(bool in_fbo);
    
    void draw_centered_string(ofTrueTypeFont * font, string text, int x, int y, bool outline=false);
    
	void exit();
	
	void keyPressed(int key);
    void toggle_mute();
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
    shared_ptr<GuiApp> gui;
    
    VideoWaves video_waves;
    
    
    //ANDY WORK
    ofFbo test_anim_fbo; //testing
    
    int game_w, game_h;
    int screen_offset_y;
    
    Game game;
    
    ParticleSystem ps;
    
    vector<Slider> sliders;
    
    bool debug_draw_test;
    
    Goal goal;
    float time_in_level;
    
    int level_num;
    string level_name;
    int num_to_save;
    
#define NUM_LEVELS 4
    string level_names[NUM_LEVELS];
    float best_times[NUM_LEVELS];
    
#define STATE_TITLE -1
#define STATE_LEVEL_SELECT -2
    
#define STATE_PLAYING 0
#define STATE_LOSE 1
#define STATE_WIN 2

    int game_state;
    
    vector<Button> buttons;
    
    ofTrueTypeFont title_font;
    ofTrueTypeFont big_font, medium_font, small_font, tiny_font;
    
    bool mute_sound;
    ofSoundPlayer win_sound, lose_sound;
    
    vector<ofSoundPlayer> menu_sounds;
    float cur_menu_sound_speed;
    int menu_sound_count;
    float menu_sound_timer;
    
    //highlighting particle changes
    float tint_time;
    float saved_tint_timer;
    float dead_tint_timer;
    
    
    string save_file_name;
    
    
};

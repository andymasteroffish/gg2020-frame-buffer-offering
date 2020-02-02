//
//  Game.hpp
//  Test_ing0
//
//  Created by Andrew Wallace on 1/31/20.
//

#ifndef Game_hpp
#define Game_hpp

#include "GuiApp.h"

#include "ofMain.h"
#include "Player.hpp"
#include "PowerSwitch.hpp"

class Game{
public:
    
    void setup(int _level_num, int screen_w, int screen_h);
    void keyPressed(int key);
    void keyReleased(int key);
    void update();
    
    void draw();
    
    void zeno(float &val, float target, float prc);
    //void zeno(int &val, float target, float prc);
    
    shared_ptr<GuiApp> gui; //fucking with the settings
    
    int level_num;
    
    int board_w, board_h;
    
    ofVec2f cam_pos;
    
    ofFbo fbo;
    
    Player player;
    bool holding_up, holding_down, holding_left, holding_right;
    
    vector<PowerSwitch> power_switches;
    
    //bool debug_draw_over;
    
    bool delay_amount_effect;
    bool rotate_effect;
    bool hue_effect;
    
    float frame_delay_f;
};

#endif /* Game_hpp */

//
//  VideoWaves.hpp
//  Test_ing0
//
//  Created by Andrei Jay
//

/*thanks to ed tannenbaum for help in fixing up the framebuffer code!*/










//so what is going to be added in here
//how many shader runs can we do?  curious if it will be feasible to do seperate shader runs for all of the vertex displacements but i think that it would be a good goal to have in terms of making a universial function for that to reuse as much as possible!  plus a goal is to test how many shader passes i can run before noticing slowdown!


#ifndef VideoWaves_hpp
#define VideoWaves_hpp

#include "ofMain.h"
#include "GuiApp.h"
#include "ofxSyphon.h"



class VideoWaves{
public:
    
    void incIndex();
    void setup(int _screen_w, int _screen_h);
    void update();
    void draw(ofFbo * input_fbo);
    
    int screen_w, screen_h;
    
    //-----------guibiz
    shared_ptr<GuiApp> gui;
    
    ofShader shader_mixer;
    ofShader shader_blur;
    ofShader shader_sharpen;
    
    ofFbo fbo_draw;
    ofFbo fbo_feedback;
    ofFbo fbo_blur;
    
    ofFbo syphonTexture;
    
    //ofVideoGrabber cam1;
    
    //secret loop option over here
    ofVideoPlayer loopMovie;
    //trysomaudiobiz
    
    ofSoundPlayer        loop;
    
    static constexpr size_t nBandsToGet = 128;
    std::array<float, nBandsToGet> fftSmoothed{{0}};
    
    //maybes try the frequency in a terrain curve
    ofPolyline frequencyLine0;
    ofPolyline frequencyLine1;
    ofPolyline frequencyLine2;
    ofPolyline frequencyLine3;
    
    ofPlanePrimitive plane;
    
    //sypphon some shit in yo
    
    ofxSyphonClient mClient;
    
    //and send it back out too
    ofxSyphonServer mainOutputSyphonServer;
    
    ofImage pnt_img;
};

#endif /* VideoWaves_hpp */

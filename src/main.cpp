
#include "ofMain.h"
#include "ofApp.h"

#include "GuiApp.h"
#include "ofAppGLFWWindow.h"
int main(){
    
//#define SHOW_GUI
    
    //--------------oldsinglewindowsetup
    /*
     ofSetupOpenGL(1024, 1024, OF_WINDOW);
     ofRunApp(new ofApp());
     */
    
    
    
    //---------------------------------
    //dualwindowsetup
    ofGLFWWindowSettings settings;
    
   // settings.setSize(1024, 1024);
    
     //settings.setSize(1280,720);
    

    settings.setSize(640, 480 + 260);
    settings.setPosition(ofVec2f(550,0));
    settings.resizable = true;
    //settings.decorated = false;
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);
    
#ifdef SHOW_GUI

        settings.setSize(550, 960);
        settings.setPosition(ofVec2f(0,0));
        settings.resizable = true;
        shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
#endif
    
    shared_ptr<ofApp> mainApp(new ofApp);
    shared_ptr<GuiApp> guiApp(new GuiApp);
    mainApp->gui = guiApp;
   // mainApp->game.gui = guiApp;
    
#ifdef SHOW_GUI
        ofRunApp(guiWindow, guiApp);
#endif
    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();
    
    
    
    
}

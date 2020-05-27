#include "ofApp.h"
#include "ofMain.h"

//========================================================================
int main() {
    // Setup the GL context.
    ofSetupOpenGL(1024, 1024, OF_WINDOW);

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new ofApp());
}

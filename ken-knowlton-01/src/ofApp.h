#pragma once

#include "ofMain.h"
#include "ofxColorQuantizer.h"
#include "ofxGaussian.h"
#include "ofxOpenCv.h"

#define ITEMS 42

class ofApp : public ofBaseApp {
  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    vector<ofImage> fragments;
    vector<vector<ofColor>> fragmentColors;
    int selectedImage;
    //        int totalFragments = 100;
    int totalFragments = 431;
//    int totalFragments = 1;
    ofImage targetImage;
    vector<ofPath> paths;

    int mosaic[ITEMS][ITEMS];
    float mosaicRotation[ITEMS][ITEMS];
    bool showOrg;
};

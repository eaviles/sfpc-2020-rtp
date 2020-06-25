#pragma once

#include "ofMain.h"
#include "ofxColorQuantizer.h"
#include "ofxGaussian.h"
#include "ofxOpenCv.h"

#define ITEMS 32
#define TOTAL_FRAGMENTS 410
#define MIN_FRG_SIZE 256

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

    void cacheFragment(int i);
    void colorFragment(ofImage img, int i);

    vector<ofImage> fragments;
    vector<ofColor> fragmentColors;
    ofImage targetImage;

    int mosaic[ITEMS][ITEMS];
    float mosaicRotation[ITEMS][ITEMS];
    bool showOrg;
    int fragmentCounters[TOTAL_FRAGMENTS];
};

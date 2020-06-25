#pragma once

#include "ofMain.h"
#include "ofxColorQuantizer.h"
#include "ofxGaussian.h"
#include "ofxOpenCv.h"

#define FRAGMENTATION 32
#define TOTAL_FRAGMENTS 410
#define MIN_FRG_SIZE 256
#define MAX_FRG_REUSE 128

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
    int indexOfFragmentByColor(ofColor color);

    vector<ofImage> fragments;
    vector<ofColor> fragmentColors;
    ofImage targetImage;

    bool showOrg;
    int fragmentCounters[TOTAL_FRAGMENTS];
};

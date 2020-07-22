#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker2.h"

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

    vector<ofImage> photos;
    vector<string> names;
    ofxFaceTracker2 tracker;
    ofxFaceTracker2 videoTracker;
    ofVideoGrabber grabber;

    int idx = 0;
    int lastIdx = -1;
    ofPoint slep;
    ofPoint srep;
};

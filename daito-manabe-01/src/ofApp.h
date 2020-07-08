#pragma once

#include "ofMain.h"
#include "ofxCv.h"

using namespace ofxCv;

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

struct fieldLine {
    float x;
    float y;
    float z;
    float l;
    float w;

    fieldLine() {
        float r = ofRandomuf();
        if (r > 0.525)
            x = ofRandom((float)IMAGE_WIDTH * 0.6, IMAGE_WIDTH);
        else if (r > 0.05)
            x = ofRandom(0, (float)IMAGE_WIDTH * 0.4);
        else
            x = ofRandom((float)IMAGE_WIDTH * 0.4, (float)IMAGE_WIDTH * 0.6);

        y = ofRandom(0, IMAGE_HEIGHT);
        z = ofRandom(2400, 2400 * 2);
        //        z = ofRandom(, 1200);
        l = ofRandom(400, 2400);

        r = ofRandomuf();
        if (r > 0.98) w = 8;
        //        else if (r > 0.8) w = 4;
        else
            w = 2;
    }
};

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

    ofVideoGrabber grabber;
    ofImage grayImg;
    ofImage diffImg;
    ofImage prevImg;
    ofImage thrsImg;
    ofImage maskImg;
    ofImage orgThrsImg;
    ofImage alphaImg;
    vector<ofPath> history;
    ContourFinder finder;
    vector<ofColor> palette;

    vector<fieldLine> fieldLines;
};

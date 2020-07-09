#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker2.h"

using namespace ofxCv;

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
#define FIELD_LINES_MASK_THRESHOLD 0.75
#define FIELD_LINES_DEPTH 2400
#define FIELD_LINES_SPEED 24

struct fieldLine {
    float x;
    float y;
    float z;
    float l;
    float w;

    fieldLine() {
        float r = ofRandomuf();
        if (r > 0.525) {
            x = ofRandom((float)IMAGE_WIDTH * 0.6, IMAGE_WIDTH);
        } else if (r > 0.05) {
            x = ofRandom(0, (float)IMAGE_WIDTH * 0.4);
        } else {
            x = ofRandom((float)IMAGE_WIDTH * 0.4, (float)IMAGE_WIDTH * 0.6);
        }

        y = ofRandom(0, IMAGE_HEIGHT);
        z = ofRandom(FIELD_LINES_DEPTH, FIELD_LINES_DEPTH * 2);
        l = ofRandom(FIELD_LINES_DEPTH * 0.25, FIELD_LINES_DEPTH);
        r = ofRandomuf();

        if (r > 0.98) {
            w = 12;
        } else {
            w = 2;
        }
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

    void drawBackground();
    void drawMotion();
    void drawBody();
    void drawFace();

    ofImage diffImg;
    ofImage grayImg;
    ofImage prevImg;
    ofImage thrsImg;
    ofImage bodyImg;
    int maxFieldLines = 512;
    float smoothNumMovPixels = 0;

    ContourFinder motionFinder;
    ContourFinder bodyFinder;
    ofVideoGrabber grabber;
    vector<fieldLine> fieldLines;
    vector<ofColor> palette;
    vector<ofPath> history;
    ofxFaceTracker2 tracker;
};

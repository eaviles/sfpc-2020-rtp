#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker2.h"

#define FRAME_RATE 60.0
#define PHOTO_DELAY 2.0

struct FaceMeasures {
    float eyes;
    float eyebrows;
    float noseBase;
    float noseBridge;
    float mouth;
    float lip;
    float jaw;
    float faceProp;
    float eyeWidth;
    float eyebrowWidth;
};

struct StudentName {
    string firstName;
    string lastName;
};

struct EyesLocation {
    ofPoint left;
    ofPoint right;
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

    FaceMeasures measureFace(ofxFaceTracker2 &tracker);
    int compareWithFaces(ofxFaceTracker2 &tracker);
    void prepareFbo(int fboIdx);

    vector<ofImage> photos;
    vector<StudentName> names;
    vector<FaceMeasures> measures;
    vector<EyesLocation> eyes;
    vector<ofImage> images;
    ofImage cameraImage;

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
    float lastCheck = 0.0;
    ofShader shader;

    int currentPhoto;
    int previousPhoto;
    float confidence;
    ofPoint slep;
    ofPoint srep;

    ofTrueTypeFont font1;
    ofTrueTypeFont font2;
};

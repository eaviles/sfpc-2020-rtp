#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxFaceTracker2.h"

struct FaceMeasures {
    float eyes;
    float eyebrows;
    float noseBase;
    float noseBridge;
    float mouth;
    float lip;
    float jaw;
    float faceProp;
};

struct StudentName {
    string firstName;
    string lastName;
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

    vector<ofImage> photos;
    vector<StudentName> names;
    vector<FaceMeasures> measures;

    ofxFaceTracker2 tracker;
    ofVideoGrabber grabber;
    int idx;
};

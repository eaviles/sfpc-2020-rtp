#pragma once

#include "ofMain.h"

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

    vector<ofImage> images;
    vector<ofImage> masks;
    vector<vector<int>> edges;
    vector<ofImage> correctedImages;
    ofImage averageImage;
    bool mouseIsDown = false;

    bool drawOriginal = true;
    bool drawMaskEdges = false;
    bool drawStraightened = false;
    bool drawAligned = false;
    bool drawAverage = false;

    bool smoothInitiated = false;
    float smoothScale = 0.0;
    float smoothAngle = 0.0;
};

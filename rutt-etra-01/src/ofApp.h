#pragma once

#include <librealsense2/rs.hpp>

#include "ofMain.h"
#include "ofxGui.h"

#define SHOW_GUI false

using namespace glm;

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

    rs2::pipeline pipe;
    rs2::config cfg;
    rs2::points points;
    rs2::pointcloud pc;
    ofEasyCam cam;
    rs2::threshold_filter thrFilter;
    rs2::temporal_filter tmpFilter;
    rs2::frameset frames;

    vector<ofMesh> meshes;
    float minX, minY, minZ, maxX, maxY, maxZ;
    vector<vec3> offsets;

    ofxPanel gui;
    ofParameter<string> fps;
    ofParameter<vec3> cameraOrientation;
    ofParameter<vec3> cameraPosition;

    ofFbo fbo;
    ofShader shader;
};

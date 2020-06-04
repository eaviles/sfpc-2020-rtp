#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    // Increase quality.
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetLineWidth(10);
    ofSetCircleResolution(32);

    ofSetBackgroundAuto(false);
    ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {

    int rings = 12;

    // Optical printing.
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(200, 205, 215);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
    ofSetColor(ofColor::white);

    float num, x;
    float dots = 4;

    for (int r = 0; r < dots; r++) {
    ofRotateDeg(360/dots);

    for (int i = 0; i < rings; i++) {
        ofRotateDeg(10);
        num = sin(ofGetElapsedTimef() * 0.1 + i * 0.3);
        x = ofMap(num, -1, 1, 100, ofGetWidth() / 2);
        ofDrawCircle(x, 0, 10);
    }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {}

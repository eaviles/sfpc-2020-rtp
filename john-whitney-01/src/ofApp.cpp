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
    float dots = 512;
    float num, x, r, g, b;
    float time = ofGetElapsedTimef();
    float width = ofGetWidth();
    float height = ofGetHeight();

    // Optical printing.
    ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);
    ofSetColor(200, 205, 215);
    ofDrawRectangle(0, 0, width, height);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);

    ofTranslate(width / 2, height / 2);

    float insideGap = cos(time * 0.75 + 3.1416 / 2);
    insideGap = ofMap(insideGap, -1, 1, width * 0.05, width * 0.2);

    for (int rd = 0; rd < dots; rd++) {
        float dotsSep = 360.0 / dots;
        ofRotateDeg(dotsSep);

        for (int i = 0; i < rings; i++) {

            float rOff = i * 0.6;
            float alpha = ofMap(sin(time * .3 + rOff * 2), -1, 1, .5, 1.0);

            r = sin(time * .5 + dotsSep * rd * .0175 + rOff + 100);
            r = ofMap(r, -1, 1, 0, 255) * alpha;
            g = cos(time * .5 + dotsSep * rd * .0175 + rOff + 10);
            g = ofMap(g, -1, 1, 0, 255) * alpha;
            b = cos(time * 0.5 + dotsSep * rd * .0175 + rOff + 10);
            b = ofMap(b, -1, 1, 0, 255) * alpha;
            ofSetColor(r, g, b);

            num = sin(ofGetElapsedTimef() * 0.5 + i * 0.2);
            x = ofMap(num, -1, 1, insideGap, width / 2);
            ofDrawCircle(x, 0, 4);
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
void ofApp::mouseReleased(int x, int y, int button) {}

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

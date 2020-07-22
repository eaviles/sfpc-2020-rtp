#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofDirectory dir;
    dir.listDir("faces");
    for (int i = 0; i < dir.size(); i++) {
        ofImage photo;
        photos.push_back(photo);
        photos.back().load(dir.getPath(i));
        names.push_back(dir.getName(i));
    }

    tracker.setup();
    tracker.setThreaded(false);
}

//--------------------------------------------------------------
void ofApp::update() {
    idx = ofMap(mouseX, 0, ofGetWidth(), 0, photos.size() - 1, true);
    if (idx != lastIdx) {
        tracker.update(photos[idx]);
        lastIdx = idx;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    photos[idx].draw(0, 50);
    ofSetColor(ofColor::white);
    ofDrawBitmapString(names[idx], 0, 20);
    ofTranslate(0, 50);
    tracker.drawDebug();
    tracker.drawDebugPose();
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

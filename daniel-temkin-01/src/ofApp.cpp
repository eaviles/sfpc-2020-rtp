#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(720, 1280);
    ofSetFrameRate(24);

    ofDirectory dir;
    dir.listDir("images");
    dir.sort();
    for (int i = 0; i < dir.size(); i++) {
        int prcLoad = round((float)i / dir.size() * 100.0);
        cout << "Loading " << prcLoad << "%" << endl;
        string filePath = dir.getPath(i);
        ofImage img;
        images.push_back(img);
        images.back().load(filePath);
    }
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);

    int width = ofGetWidth();
    int imgsCount = images.size();

    int imgIdx = 0;

    if (useAutoPlay) {
        imgIdx = ofGetFrameNum() % (imgsCount * 2);
        if (imgIdx > imgsCount - 1) {
            imgIdx = imgsCount * 2 - imgIdx - 1;
        }
    }

    if (useMousePlay) {
        imgIdx = ofMap(mouseX, 0, width, 0, imgsCount - 1, true);
    }

    images[imgIdx].draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == '1') {
        useAutoPlay = false;
        useMousePlay = true;
    } else if (key == '2') {
        useAutoPlay = true;
        useMousePlay = false;
    }
}

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

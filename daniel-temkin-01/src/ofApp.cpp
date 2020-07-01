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
        auto filePathParts = ofSplitString(filePath, "/");
        string maskFilePath = "masks/" + filePathParts[1];
        ofImage img;
        images.push_back(img);
        images.back().load(filePath);
        ofImage maskImg;
        masks.push_back(maskImg);
        masks.back().load(maskFilePath);
    }
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);

    int width = ofGetWidth();
    int imgsCount = images.size();

    // Get the current image index for auto-play or for the mouse position.
    int imgIdx = 0;
    if (!mouseIsDown) {
        imgIdx = ofGetFrameNum() % (imgsCount * 2);
        if (imgIdx > imgsCount - 1) {
            imgIdx = imgsCount * 2 - imgIdx - 1;
        }
    } else {
        imgIdx = ofMap(mouseX, 0, width, 0, imgsCount - 1, true);
    }

    images[imgIdx].draw(0, 0);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 128);
    masks[imgIdx].draw(0, 0);
    ofDisableAlphaBlending();
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
void ofApp::mousePressed(int x, int y, int button) { mouseIsDown = true; }

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) { mouseIsDown = false; }

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

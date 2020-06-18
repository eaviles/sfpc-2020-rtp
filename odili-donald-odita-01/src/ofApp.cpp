#include "ofApp.h"

#include "ofxGaussian.h"

ofColor getRandomColor() {
    float r = ofxGaussian() * 1.5;
    vector<ofColor> colors = {
        ofColor::fromHex(0x02123e), ofColor::fromHex(0x052a5e),
        ofColor::fromHex(0x023978), ofColor::fromHex(0x253b6d),
        ofColor::fromHex(0x0257a7), ofColor::fromHex(0x0192be),
        ofColor::fromHex(0x0081c4), ofColor::fromHex(0x0095d9),
        ofColor::fromHex(0x4c8fb4), ofColor::fromHex(0x0ca9da),
        ofColor::fromHex(0x66b9e1), ofColor::fromHex(0x80c7eb),
        ofColor::fromHex(0xafd5ec), ofColor::fromHex(0x94bed6),
        ofColor::fromHex(0xa6e3de), ofColor::fromHex(0x6fb8af),
        ofColor::fromHex(0x40bdb7), ofColor::fromHex(0x339b95),
        ofColor::fromHex(0x318f15), ofColor::fromHex(0x5db924),
        ofColor::fromHex(0xd8d82a), ofColor::fromHex(0xdba822),
        ofColor::fromHex(0xafaa45), ofColor::fromHex(0xeb961f),
        ofColor::fromHex(0xfdbf27), ofColor::fromHex(0xf1c523),
        ofColor::fromHex(0xfbdf74), ofColor::fromHex(0xfdd42c),
        ofColor::fromHex(0xffe8a4), ofColor::fromHex(0xd17b64),
        ofColor::fromHex(0xf34d49), ofColor::fromHex(0xda2e3b),
        ofColor::fromHex(0xc32240), ofColor::fromHex(0xaf4f7b),
        ofColor::fromHex(0xb51e41), ofColor::fromHex(0xd269a4),
        ofColor::fromHex(0x871314), ofColor::fromHex(0x912139)};
    int n = (int)ofMap(r, -3.0, 3.0, 0, colors.size() - 1, true);
//    int n = (int)ofMap(ofRandom(1.0), 0.0, 1.0, 0, colors.size() - 1, true);
    return colors[n];
}

void ofApp::addLeftPath(float left, float top) {
    float cellWidth = 90.0;
    float cellHeight = 210.0;
    ofPath path;

    path = ofPath();
    path.setColor(getRandomColor());
    path.moveTo(left, top);
    path.lineTo(left + cellWidth, top + cellHeight * 2.0);
    path.lineTo(left, top + cellHeight);
    paths.push_back(path);

    path = ofPath();
    path.setColor(getRandomColor());
    path.moveTo(left, top);
    path.lineTo(left + cellWidth, top + cellHeight * 2.0);
    path.lineTo(left + cellWidth, top + cellHeight);
    paths.push_back(path);
}

void ofApp::addRightPath(float left, float top) {
    float cellWidth = 90.0;
    float cellHeight = 210.0;
    ofPath path;

    path = ofPath();
    path.setColor(getRandomColor());
    path.moveTo(left + cellWidth, top);
    path.lineTo(left, top + cellHeight * 2.0);
    path.lineTo(left, top + cellHeight);
    paths.push_back(path);

    path = ofPath();
    path.setColor(getRandomColor());
    path.moveTo(left + cellWidth, top);
    path.lineTo(left, top + cellHeight * 2.0);
    path.lineTo(left + cellWidth, top + cellHeight);
    paths.push_back(path);
}

void ofApp::addLeftRhumbus(float left, float top) {
    float cellWidth = 90.0;
    float cellHeight = 210.0;

    ofPath p = ofPath();
    p.setColor(getRandomColor());
    p.moveTo(left, top);
    p.lineTo(left + cellWidth, top + cellHeight);
    p.lineTo(left + cellWidth, top);
    p.lineTo(left, top - cellHeight);
    paths.push_back(p);
}

void ofApp::addRightRhumbus(float left, float top) {
    float cellWidth = 90.0;
    float cellHeight = 210.0;

    ofPath p = ofPath();
    p.setColor(getRandomColor());
    p.moveTo(left, top);
    p.lineTo(left + cellWidth, top - cellHeight);
    p.lineTo(left + cellWidth, top);
    p.lineTo(left, top + cellHeight);
    paths.push_back(p);
}

void ofApp::generateData() {
    paths.clear();
    float cellWidth = 90.0;
    float cellHeight = 210.0;
    float windowWidth = ofGetWidth();
    float windowHeight = ofGetHeight();

    enum Direction { noDir, leftDir, rightDir };
    Direction lastDir = noDir;

    float top = 0.0;
    float left = 0.0;
    while (left < windowWidth) {
        lastDir = noDir;

        bool skipTop = (int)(left / cellWidth) % 3 == 0;
        if (ofRandom(1.0) > 0.95) skipTop = !skipTop;

        if (skipTop) {
            top = 0.0;
        } else {
            top = cellHeight;
        }

        while (top < windowHeight) {

            bool doLeft = (int)(top / (cellHeight * 2)) % 2;
            if ((int)(left / cellWidth) % 2 == 1) doLeft = !doLeft;
            if (ofRandom(1.0) > 0.9) doLeft = !doLeft;

            if (doLeft) {
                addLeftPath(left, top);
                if (lastDir == leftDir && ofRandom(1.0) < 0.25) {
                    addLeftRhumbus(left, top);
                }
                lastDir = leftDir;
            } else {
                addRightPath(left, top);
                if (lastDir == rightDir && ofRandom(1.0) < 0.25) {
                    addRightRhumbus(left, top);
                }
                lastDir = rightDir;
            }
            top += cellHeight * 2.0;
        }
        left += cellWidth;
    }

    //    ofPath p = ofPath();
    //    p.moveTo(100,100);
    //    p.lineTo(200, 200);
    //    p.lineTo(140,180);
    //    paths.push_back(p);
    //    colors.push_back(ofColor::white);
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    generateData();
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(245);
    for (int i = 0; i < paths.size(); i++) {
        paths[i].draw();
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
void ofApp::mouseReleased(int x, int y, int button) { generateData(); }

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

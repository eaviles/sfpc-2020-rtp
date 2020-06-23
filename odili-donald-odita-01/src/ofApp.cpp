#include "ofApp.h"

#include "ofxGaussian.h"

int palette[] = {0x02123e, 0x052a5e, 0x023978, 0x253b6d, 0x0257a7, 0x0192be,
                 0x0081c4, 0x0095d9, 0x4c8fb4, 0x0ca9da, 0x66b9e1, 0x80c7eb,
                 0xafd5ec, 0x94bed6, 0xa6e3de, 0x6fb8af, 0x40bdb7, 0x339b95,
                 0x318f15, 0x5db924, 0xd8d82a, 0xdba822, 0xafaa45, 0xeb961f,
                 0xfdbf27, 0xf1c523, 0xfbdf74, 0xfdd42c, 0xffe8a4, 0xd17b64,
                 0xf34d49, 0xda2e3b, 0xc32240, 0xaf4f7b, 0xb51e41, 0xd269a4,
                 0x871314, 0x912139};

vector<ofPath> paths;
vector<ofColor> colors;
float cellWidth = 90.0;
float cellHeight = 210.0;
float useRandGaussColors = true;
float useRandSkips = true;
float useRandFlips = true;
float useRandFills = true;
float useMonochrome = false;

ofColor getRandomColor() {
    int randColor;
    if (useRandGaussColors) {
        float randNum = ofxGaussian() * 1.5;
        randColor = ofMap(randNum, -3.0, 3.0, 0, colors.size() - 1, true);
    } else {
        float randNum = ofRandomuf();
        randColor = ofMap(randNum, 0.0, 1.0, 0, colors.size() - 1);
    }
    return colors[randColor];
}

string debugBool(bool value) { return value ? "true" : "false"; }

void ofApp::addLeftPath(float left, float top) {
    ofPath path;

    path = ofPath();
    path.setColor(useMonochrome ? ofColor::black : getRandomColor());
    path.moveTo(left, top);
    path.lineTo(left + cellWidth, top + cellHeight * 2.0);
    path.lineTo(left, top + cellHeight);
    paths.push_back(path);

    path = ofPath();
    path.setColor(useMonochrome ? ofColor::white : getRandomColor());
    path.moveTo(left, top);
    path.lineTo(left + cellWidth, top + cellHeight * 2.0);
    path.lineTo(left + cellWidth, top + cellHeight);
    paths.push_back(path);
}

void ofApp::addRightPath(float left, float top) {
    ofPath path;

    path = ofPath();
    path.setColor(useMonochrome ? ofColor::black : getRandomColor());
    path.moveTo(left + cellWidth, top);
    path.lineTo(left, top + cellHeight * 2.0);
    path.lineTo(left, top + cellHeight);
    paths.push_back(path);

    path = ofPath();
    path.setColor(useMonochrome ? ofColor::white : getRandomColor());
    path.moveTo(left + cellWidth, top);
    path.lineTo(left, top + cellHeight * 2.0);
    path.lineTo(left + cellWidth, top + cellHeight);
    paths.push_back(path);
}

void ofApp::addLeftRhumbus(float left, float top) {
    ofPath p = ofPath();
    p.setColor(useMonochrome ? ofColor::black : getRandomColor());
    p.moveTo(left, top);
    p.lineTo(left + cellWidth, top + cellHeight);
    p.lineTo(left + cellWidth, top);
    p.lineTo(left, top - cellHeight);
    paths.push_back(p);
}

void ofApp::addRightRhumbus(float left, float top) {
    ofPath p = ofPath();
    p.setColor(useMonochrome ? ofColor::black : getRandomColor());
    p.moveTo(left, top);
    p.lineTo(left + cellWidth, top - cellHeight);
    p.lineTo(left + cellWidth, top);
    p.lineTo(left, top + cellHeight);
    paths.push_back(p);
}

void ofApp::generateData() {
    paths.clear();

    float windowWidth = ofGetWidth();
    float windowHeight = ofGetHeight();

    enum Direction { noDir, leftDir, rightDir };
    Direction lastDir = noDir;

    float top = 0.0;
    float left = 0.0;
    while (left < windowWidth) {
        lastDir = noDir;

        bool skipTop = (int)(left / cellWidth) % 3 == 0;
        if (useRandSkips && ofRandomuf() > 0.8) skipTop = !skipTop;

        if (skipTop) {
            top = 0.0;
        } else {
            top = cellHeight;
        }

        while (top < windowHeight) {

            bool doLeft = (int)(top / (cellHeight * 2)) % 2;
            if ((int)(left / cellWidth) % 2 == 1) doLeft = !doLeft;
            if (useRandFlips && ofRandomuf() > 0.8) doLeft = !doLeft;

            if (doLeft) {
                addLeftPath(left, top);
                if (useRandFills && lastDir == leftDir && ofRandomuf() < 0.25) {
                    addLeftRhumbus(left, top);
                }
                lastDir = leftDir;
            } else {
                addRightPath(left, top);
                if (useRandFills && lastDir == rightDir &&
                    ofRandomuf() < 0.25) {
                    addRightRhumbus(left, top);
                }
                lastDir = rightDir;
            }
            top += cellHeight * 2.0;
        }
        left += cellWidth;
    }
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();

    int numOfColors = sizeof(palette) / sizeof(palette[0]);
    for (int i = 0; i < numOfColors; i++) {
        colors.push_back(ofColor::fromHex(palette[i]));
    }

    generateData();
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(useMonochrome ? 128 : 245);
    for (int i = 0; i < paths.size(); i++) {
        paths[i].draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 49) {
        useRandGaussColors = !useRandGaussColors;
    } else if (key == 50) {
        useRandSkips = !useRandSkips;
    } else if (key == 51) {
        useRandFlips = !useRandFlips;
    } else if (key == 52) {
        useRandFills = !useRandFills;
    } else if (key == 53) {
        useMonochrome = !useMonochrome;
    } else if (key == 57) {
        useRandGaussColors = false;
        useRandSkips = false;
        useRandFlips = false;
        useRandFills = false;
        useMonochrome = true;
        generateData();
    } else if (key == 48) {
        useRandGaussColors = true;
        useRandSkips = true;
        useRandFlips = true;
        useRandFills = true;
        useMonochrome = false;
    }
    cout << "1. useRandGauss? " << debugBool(useRandGaussColors) << endl;
    cout << "2. useRandSkips? " << debugBool(useRandSkips) << endl;
    cout << "3. useRandFlips? " << debugBool(useRandFlips) << endl;
    cout << "4. useRandFills? " << debugBool(useRandFills) << endl;
    cout << "5. useMonochrome? " << debugBool(useMonochrome) << endl;
    cout << "9. basic structure" << endl;
    cout << "0. reset all" << endl;
    cout << endl;
    generateData();
}

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

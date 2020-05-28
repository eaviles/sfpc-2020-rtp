#include "ofApp.h"

//--------------------------------------------------------------
ofVec2f findCoordForValue(Cell square[4][4], int value) {
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            Cell c = square[x][y];
            if (c.v == value) {
                return ofVec2f(c.y, c.x);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::addSquareLinePoints(Cell square[4][4]) {
    for (int i = 0; i < 16; i++) {
        ofVec2f coord = findCoordForValue(square, i + 1);
        linePoints.push_back(coord);
    }
}

//--------------------------------------------------------------
void ofApp::generateData() {
    Cell cells[5][5][4][4];
    float margin = 40.0;
    float squareSize = 156.8;
    float pointSep = squareSize / 3.0;

    // Generate the cell data for each square.
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {

            // Get a shuffled vector with numbers from 1 to 16.
            vector<int> v;
            v.resize(16);
            for (int j = 0; j < v.size(); j++) {
                v[j] = j + 1;
            }
            ofRandomize(v);

            // Resolve the coordinates for each cell.
            float dx = (row + 1) * margin + row * squareSize;
            float dy = (col + 1) * margin + col * squareSize;
            for (int x = 0; x < 4; x++) {
                for (int y = 0; y < 4; y++) {
                    int z = x * 4 + y;
                    float px = x * pointSep;
                    float py = y * pointSep;
                    Cell &c = cells[row][col][x][y];
                    c.v = v[z];
                    c.x = dx + px + ofRandom(-3.0, 3.0);
                    c.y = dy + py + ofRandom(-3.0, 3.0);
                }
            }
        }
    }

    // With the cells data, build the lines.
    linePoints.clear();
    for (int row = 0; row < 5; row++) {
        if (row % 2 == 0) {
            for (int col = 0; col < 5; col++) {
                addSquareLinePoints(cells[row][col]);
            }
        } else {
            for (int col = 4; col >= 0; col--) {
                addSquareLinePoints(cells[row][col]);
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetCircleResolution(128);
    ofSetLineWidth(3);
    generateData();
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(18, 26, 33);
    ofSetColor(222, 223, 226, 255 * 0.75);
    for (int i = 0; i < linePoints.size() - 1; i++) {
        ofVec2f from = linePoints[i];
        ofVec2f to = linePoints[i + 1];
        ofDrawLine(from, to);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 's' || key == 'S') {
        ofImage image;
        image.grabScreen(0, 0, 1024, 1024);
        image.saveImage("output.jpg");
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

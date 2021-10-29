#include "ofApp.h"

// Magic Square Transformations --------------------------------

magicSquare rotateSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[12];
    r[1] = s[8];
    r[2] = s[4];
    r[3] = s[0];
    r[4] = s[13];
    r[5] = s[9];
    r[6] = s[5];
    r[7] = s[1];
    r[8] = s[14];
    r[9] = s[10];
    r[10] = s[6];
    r[11] = s[2];
    r[12] = s[15];
    r[13] = s[11];
    r[14] = s[7];
    r[15] = s[3];
    return r;
}

magicSquare rotateSquareNTimes(magicSquare s) {
    int times = rand() % 3 + 1;
    array<int, 16> r = s;
    for (int i = 0; i < times; i++) {
        r = rotateSquare(r);
    }
    return r;
}

magicSquare horizontalReflectSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[3];
    r[1] = s[2];
    r[2] = s[1];
    r[3] = s[0];
    r[4] = s[7];
    r[5] = s[6];
    r[6] = s[5];
    r[7] = s[4];
    r[8] = s[11];
    r[9] = s[10];
    r[10] = s[9];
    r[11] = s[8];
    r[12] = s[15];
    r[13] = s[14];
    r[14] = s[13];
    r[15] = s[12];
    return r;
}

magicSquare verticalReflectSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[12];
    r[1] = s[13];
    r[2] = s[14];
    r[3] = s[15];
    r[4] = s[8];
    r[5] = s[9];
    r[6] = s[10];
    r[7] = s[11];
    r[8] = s[4];
    r[9] = s[5];
    r[10] = s[6];
    r[11] = s[7];
    r[12] = s[0];
    r[13] = s[1];
    r[14] = s[2];
    r[15] = s[3];
    return r;
}

magicSquare intersectComplementSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[15];
    r[1] = s[13];
    r[14] = s[2];
    r[3] = s[12];
    r[4] = s[7];
    r[5] = s[5];
    r[6] = s[6];
    r[7] = s[4];
    r[8] = s[11];
    r[9] = s[9];
    r[10] = s[10];
    r[11] = s[8];
    r[12] = s[3];
    r[13] = s[1];
    r[2] = s[14];
    r[15] = s[0];
    return r;
}

magicSquare interchangeColumnsSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[1];
    r[1] = s[0];
    r[2] = s[3];
    r[3] = s[2];
    r[4] = s[5];
    r[5] = s[4];
    r[6] = s[7];
    r[7] = s[6];
    r[8] = s[9];
    r[9] = s[8];
    r[10] = s[11];
    r[11] = s[10];
    r[12] = s[13];
    r[13] = s[12];
    r[14] = s[15];
    r[15] = s[14];
    return r;
}

magicSquare interchangeRowsSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[4];
    r[1] = s[5];
    r[2] = s[6];
    r[3] = s[7];
    r[4] = s[0];
    r[5] = s[1];
    r[6] = s[2];
    r[7] = s[3];
    r[8] = s[12];
    r[9] = s[13];
    r[10] = s[14];
    r[11] = s[15];
    r[12] = s[8];
    r[13] = s[9];
    r[14] = s[10];
    r[15] = s[11];
    return r;
}

magicSquare interchangeCenterColumnsSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[0];
    r[1] = s[2];
    r[2] = s[1];
    r[3] = s[3];
    r[4] = s[4];
    r[5] = s[6];
    r[6] = s[5];
    r[7] = s[7];
    r[8] = s[8];
    r[9] = s[10];
    r[10] = s[9];
    r[11] = s[11];
    r[12] = s[12];
    r[13] = s[14];
    r[14] = s[13];
    r[15] = s[15];
    return r;
}

magicSquare interchangeCenterRowsSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[0];
    r[1] = s[1];
    r[2] = s[2];
    r[3] = s[3];
    r[4] = s[8];
    r[5] = s[9];
    r[6] = s[10];
    r[7] = s[11];
    r[8] = s[4];
    r[9] = s[5];
    r[10] = s[6];
    r[11] = s[7];
    r[12] = s[12];
    r[13] = s[13];
    r[14] = s[14];
    r[15] = s[15];
    return r;
}

magicSquare interchangeQuadsSquare(magicSquare s) {
    magicSquare r;
    r[0] = s[10];
    r[1] = s[11];
    r[2] = s[8];
    r[3] = s[9];
    r[4] = s[14];
    r[5] = s[15];
    r[6] = s[12];
    r[7] = s[13];
    r[8] = s[2];
    r[9] = s[3];
    r[10] = s[0];
    r[11] = s[1];
    r[12] = s[6];
    r[13] = s[7];
    r[14] = s[4];
    r[15] = s[5];
    return r;
}

magicSquare transformSquare(magicSquare s) {
    int times = rand() % 12;
    magicSquare r = s;
    for (int i = 0; i < times; i++) {
        int type = rand() % 12;
        switch (type) {
            case 0: r = rotateSquareNTimes(r); break;
            case 1: r = horizontalReflectSquare(r); break;
            case 2: r = verticalReflectSquare(r); break;
            case 3: r = intersectComplementSquare(r); break;
            case 4: r = interchangeColumnsSquare(r); break;
            case 5: r = interchangeRowsSquare(r); break;
            case 6:
                r = interchangeColumnsSquare(r);
                r = interchangeRowsSquare(r);
                break;
            case 7: r = interchangeCenterColumnsSquare(r); break;
            case 8: r = interchangeCenterRowsSquare(r); break;
            case 9:
                r = interchangeCenterColumnsSquare(r);
                r = interchangeCenterRowsSquare(r);
                break;
            case 10: r = interchangeQuadsSquare(r); break;
        }
    }
    return r;
}

// Other Functions ---------------------------------------------

void printSquare(magicSquare m) {
    for (int i = 0; i < 4; i += 1) {
        for (int j = 0; j < 4; j += 1) {
            int v = m[i * 4 + j];
            if (v < 10) cout << " ";
            cout << v << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int findValueInSquare(magicSquare s, int value) {
    for (int i = 0; i < 16; i++) {
        if (s[i] == value) return i;
    }
    return -1;
}

ofVec2f indexToCoord(int index) {
    float sep = 156.8 / 3.0;
    int j = index % 4;
    int i = index / 4;
    return ofVec2f(j * sep, i * sep);
}

void drawSquare(magicSquare s) {
    int fromIdx, toIdx;
    ofVec2f from, to;

    fromIdx = findValueInSquare(s, 1);
    from = indexToCoord(fromIdx);

    for (int i = 1; i < 16; i++) {
        toIdx = findValueInSquare(s, i + 1);
        to = indexToCoord(toIdx);
        ofDrawLine(from, to);
        from = to;
    }
}

void drawCircle() {
    ofSetColor(ofColor::white);
    ofDrawCircle(200, 200, 10);
}

//--------------------------------------------------------------
void ofApp::setup() {
    // Increase quality.
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetCircleResolution(128);

    ofSetLineWidth(2.0);

    randomSquares[0] = transformSquare(durerSquare);
    for (int i = 1; i < 25; i++) {
        randomSquares[i] = transformSquare(randomSquares[i - 1]);
    }
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(18, 26, 33);
    ofSetColor(222, 223, 226);

    float w = ofGetWindowWidth();
    float h = ofGetWindowHeight();

    float m = 40.0;
    float s = 156.8;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            ofPushMatrix();
            float x = i * s + m * (i + 1);
            float y = j * s + m * (j + 1);
            ofTranslate(x, y);
            magicSquare sq = randomSquares[i * 5 + j];
            drawSquare(sq);
            ofPopMatrix();
        }
    }

    ofVec2f from, to;

    for (int k = 0; k < 4; k++) {
        from = indexToCoord(findValueInSquare(randomSquares[k], 16));
        to = indexToCoord(findValueInSquare(randomSquares[k + 1], 1));
        ofDrawLine(from.x + m * (k + 1) + s * k, from.y + m,
                   to.x + m * (k + 2) + s * (k + 1), to.y + m);
    }

    from = indexToCoord(findValueInSquare(randomSquares[4], 16));
    to = indexToCoord(findValueInSquare(randomSquares[9], 1));
    ofDrawLine(from.x + m * 5 + s * 4, from.y + m, to.x + m * 5 + s * 4,
               to.y + m * 2 + s);

    for (int k = 9; k > 5; k--) {
        from = indexToCoord(findValueInSquare(randomSquares[k], 16));
        to = indexToCoord(findValueInSquare(randomSquares[k - 1], 1));
        ofDrawLine(from.x + m * (k - 4) + s * (k - 5), from.y + m * 2 + s,
                   to.x + m * (k - 5) + s * (k - 6), to.y + m * 2 + s);
    }

    from = indexToCoord(findValueInSquare(randomSquares[5], 16));
    to = indexToCoord(findValueInSquare(randomSquares[10], 1));
    ofDrawLine(from.x + m, from.y + m * 2 + s, to.x + m, to.y + m * 3 + s * 2);

    for (int k = 10; k < 14; k++) {
        from = indexToCoord(findValueInSquare(randomSquares[k], 16));
        to = indexToCoord(findValueInSquare(randomSquares[k + 1], 1));
        ofDrawLine(from.x + m * (k - 9) + s * (k - 10), from.y + m * 3 + s * 2,
                   to.x + m * (k - 8) + s * (k - 9), to.y + m * 3 + s * 2);
    }

    from = indexToCoord(findValueInSquare(randomSquares[14], 16));
    to = indexToCoord(findValueInSquare(randomSquares[19], 1));
    ofDrawLine(from.x + m * 5 + s * 4, from.y + m * 3 + s * 2,
               to.x + m * 5 + s * 4, to.y + m * 4 + s * 3);

    for (int k = 19; k > 15; k--) {
        from = indexToCoord(findValueInSquare(randomSquares[k], 16));
        to = indexToCoord(findValueInSquare(randomSquares[k - 1], 1));
        ofDrawLine(from.x + m * (k - 14) + s * (k - 15), from.y + m * 4 + s * 3,
                   to.x + m * (k - 15) + s * (k - 16), to.y + m * 4 + s * 3);
    }

    from = indexToCoord(findValueInSquare(randomSquares[15], 16));
    to = indexToCoord(findValueInSquare(randomSquares[20], 1));
    ofDrawLine(from.x + m, from.y + m * 4 + s * 3, to.x + m,
               to.y + m * 5 + s * 4);

    for (int k = 20; k < 24; k++) {
        from = indexToCoord(findValueInSquare(randomSquares[k], 16));
        to = indexToCoord(findValueInSquare(randomSquares[k + 1], 1));
        ofDrawLine(from.x + m * (k - 19) + s * (k - 20), from.y + m * 5 + s * 4,
                   to.x + m * (k - 18) + s * (k - 19), to.y + m * 5 + s * 4);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    if (key == 's' || key == 'S') {
        ofImage image;
        image.grabScreen(0, 0, 1024, 1024);
        image.save("output-" + ofGetTimestampString() + ".jpg");
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//---ss-----------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    randomSquares[0] = transformSquare(durerSquare);
    for (int i = 1; i < 25; i++) {
        randomSquares[i] = transformSquare(randomSquares[i - 1]);
    }
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

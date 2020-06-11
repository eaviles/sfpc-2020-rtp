#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofEnableAlphaBlending();
    ofEnableAntiAliasing();
    ofEnableSmoothing();
    ofSetLineWidth(2.0);

    ofTrueTypeFontSettings settings("nicokaku_v1.ttf", 120);
    settings.antialiased = true;
    settings.contours = true;
    settings.dpi = 72;
    settings.addRanges(ofAlphabet::Japanese);
    font1.load(settings);
    font1.setLetterSpacing(0.7);

    settings = ofTrueTypeFontSettings("MPLUS1p-ExtraBold.ttf", 120);
    settings.antialiased = true;
    settings.contours = true;
    settings.dpi = 72;
    settings.addRanges(ofAlphabet::Latin);
    font2.load(settings);
    font2.setLetterSpacing(0.9);
}

//--------------------------------------------------------------
void ofApp::update() {}

void targetRect(ofRectangle originalRectangle, ofRectangle target) {
    float sx = target.getWidth() / originalRectangle.getWidth();
    float sy = target.getHeight() / originalRectangle.getHeight();
    ofTranslate(target.getPosition());
    ofScale(sx, sy);
    ofTranslate(0, originalRectangle.getHeight());
}

//--------------------------------------------------------------
void ofApp::draw() {

    ofBackground(242);
    string msg1 = "モリサワ";
    ofRectangle rect1 = font1.getStringBoundingBox(msg1, 0, 0);
    string msg2 = "MORISAWA";
    ofRectangle rect2 = font2.getStringBoundingBox(msg2, 0, 0);

    ofTranslate(0, 10);

    int rows = 104;
    for (int r = 0; r < rows; r++) {
        float ease = ofMap(r, 0, rows - 1, 1.0, 0.0);
        float height = 400 * ofMap(ease, 1.0, 0.08, 1.0, 0.0, true);
        if (r > 0) ofTranslate(0, 24 * ease + 0.1);
        ofRectangle target(-10, 0, ofGetWidth() * 0.99, height);

        ofPushMatrix();
        targetRect(rect1, target);
        vector<ofPath> paths = font1.getStringAsPoints(msg1);
        for (int i = 0; i < paths.size(); i++) {
            if (r > 0) {
                ofSetColor(0, 0, 0, 48);
                paths[i].setPolyWindingMode(OF_POLY_WINDING_ODD);
                vector<ofPolyline> lines = paths[i].getOutline();
                for (int j = 0; j < lines.size(); j++) {
                    lines[j].setClosed(true);
                    lines[j].getResampledBySpacing(4);
                    lines[j].draw();
                }
            } else {
                ofSetColor(24);
                paths[i].draw();
            }
        }
        ofPopMatrix();
    }

    rows = 81;
    for (int r = 0; r < rows; r++) {
        float ease = ofMap(r, rows - 1, 0, 1.0, 0.0);
        float height = 240 * ofMap(ease, 1.0, 0.1, 1.0, 0.0, true) + 2;
        if (r > 0) ofTranslate(0, 24 * ease);
        ofRectangle target(29 + ofGetWidth() * 0.978, height,
                           -ofGetWidth() * 0.978, -height);

        ofPushMatrix();
        targetRect(rect2, target);
        vector<ofPath> paths = font2.getStringAsPoints(msg2);
        for (int i = 0; i < paths.size(); i++) {
            ofSetColor(0, 0, 0, 52);
            paths[i].setPolyWindingMode(OF_POLY_WINDING_ODD);
            vector<ofPolyline> lines = paths[i].getOutline();
            for (int j = 0; j < lines.size(); j++) {
                lines[j].setClosed(true);
                lines[j].getResampledBySpacing(4);
                lines[j].draw();
            }
        }
        ofPopMatrix();
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

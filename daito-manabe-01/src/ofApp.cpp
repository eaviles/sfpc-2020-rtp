#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(targetWidth * 2, targetHeight * 2);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    grabber.setDeviceID(2);
    grabber.setup(targetWidth, targetHeight);

    grayImg.allocate(targetWidth, targetHeight, OF_IMAGE_GRAYSCALE);
    diffImg.allocate(targetWidth, targetHeight, OF_IMAGE_GRAYSCALE);
    prevImg.allocate(targetWidth, targetHeight, OF_IMAGE_GRAYSCALE);
    thrsImg.allocate(targetWidth, targetHeight, OF_IMAGE_GRAYSCALE);

    palette.push_back(ofColor::red);
    palette.push_back(ofColor::magenta);
    palette.push_back(ofColor::yellow);
    palette.push_back(ofColor::green);
    palette.push_back(ofColor::blue);

    finder.setMinArea(targetWidth * 0.5);
    //    finder.setSortBySize(true);
}

//--------------------------------------------------------------
void ofApp::update() {
    grabber.update();

    if (grabber.isFrameNew()) {
        convertColor(grabber, grayImg, CV_RGB2GRAY);
        grayImg.update();

        absdiff(grayImg, prevImg, diffImg);
        diffImg.update();

        thrsImg.setFromPixels(diffImg.getPixels());
        threshold(thrsImg, 256 * 0.5);
        thrsImg.update();

        prevImg.setFromPixels(grayImg.getPixels());
        prevImg.update();

        finder.findContours(thrsImg);

        ofPath p;
        for (int i = 0; i < finder.size(); i++) {
            auto pl = finder.getPolyline(i).getResampledByCount(4);
            for (int j = 0; j < pl.getVertices().size(); j++) {
                auto v = pl.getVertices()[j];
                float rx = ofRandom(-16, 16);
                float ry = ofRandom(-16, 16);
                if (j == 0) {
                    p.newSubPath();
                    p.moveTo(v.x + rx, v.y + ry);
                } else {
                    p.lineTo(v.x + rx, v.y + ry);
                }
            }
        }
        p.close();
        p.simplify();
        history.push_back(p);

        if (history.size() > palette.size()) {
            history.erase(history.begin());
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);
    ofScale(2.0);

    //    grabber.draw(0, 0);
    //
    //    grayImg.draw(targetWidth, 0);
    //
    //    diffImg.draw(targetWidth * 2, 0);
    //
    //    thrsImg.draw(targetWidth * 3, 0);
    //
    //    ofPushMatrix();
    //    if (finder.size() > 0) {
    //        ofTranslate(targetWidth * 4, 0);
    //        finder.draw();
    //    }
    //    ofPopMatrix();
    //
    //    ofPushMatrix();
    //    ofTranslate(0, 0);
    //    for (int i = 0; i < history.size(); i++) {
    //        ofColor c = palette[i];
    //        c.a = 220;
    //        history[i].setColor(c);
    //        history[i].draw();
    //    }
    //    ofPopMatrix();
    //

    grayImg.draw(0, 0);

    for (int i = 0; i < history.size(); i++) {
        history[i].setColor(palette[i]);
        history[i].draw();
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

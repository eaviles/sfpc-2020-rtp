#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    grabber.setDeviceID(1);
    grabber.setup(IMAGE_WIDTH, IMAGE_HEIGHT);

    grayImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    diffImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    prevImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    thrsImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    orgThrsImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    alphaImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_COLOR_ALPHA);
    maskImg.load("mask.png");

    palette.push_back(ofColor::white);
    palette.push_back(ofColor::magenta);
    palette.push_back(ofColor::red);
    palette.push_back(ofColor::yellow);
    palette.push_back(ofColor::green);
    palette.push_back(ofColor::blue);

    finder.setMinArea((float)IMAGE_WIDTH * 0.35);

    for (int i = 0; i < 2048; i++) {
        fieldLines.push_back(fieldLine());
    }
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

        auto pxs = grayImg.getPixels();
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            for (int y = 0; y < IMAGE_HEIGHT; y++) {
                auto p = pxs.getColor(x, y);
                ofColor c = ofColor(p.r, p.g, p.b, p.r > 128 ? 255 : 0);
                alphaImg.setColor(x, y, c);
            }
        }
        alphaImg.update();

        prevImg.setFromPixels(grayImg.getPixels());
        prevImg.update();

        finder.findContours(thrsImg);

        ofPath p;
        for (int i = 0; i < finder.size(); i++) {
            auto pl = finder.getPolyline(i).getResampledByCount(3);
            for (int j = 0; j < pl.getVertices().size(); j++) {
                auto v = pl.getVertices()[j];
                float rx = ofRandom(-32, 32);
                float ry = ofRandom(-32, 32);
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
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);
    ofScale(2.0);

    for (int i = 0; i < fieldLines.size(); i++) {
        if (i == fieldLines.size() * 0.75) {
            maskImg.draw(0, 0);
        }
        fieldLine l = fieldLines[i];
        if (l.z - l.l > -2400) {
            ofSetLineWidth(l.w);
            ofDrawLine(l.x, l.y, l.z, l.x, l.y, l.z - l.l);
            fieldLines[i].z = l.z - 24;
        } else {
            fieldLines[i] = fieldLine();
        }
    }

    //    cout << mouseX << " - " << (mouseX + 200) << endl;

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

    //    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    alphaImg.draw(0, 0);
    //    ofDisableBlendMode();

    //    ofSetLineWidth(8.0);
    //    for (int i = 0; i < finder.size(); i++) {
    //        auto p = finder.getPolyline(i).getSmoothed(10);
    //        p.setClosed(true);
    //        p.draw();
    //    }

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

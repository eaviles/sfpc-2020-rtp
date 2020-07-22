#include "ofApp.h"

ofPoint getCenterOfFeature(ofxFaceTracker2 &tracker, ofxFaceTracker2Landmarks::Feature feature) {
    auto points = tracker.getInstances()[0].getLandmarks().getImageFeature(feature);
    ofPoint center;
    for (int i = 0; i < points.size(); center += points[i++]);
    center /= (float)points.size();
    return center;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofDirectory dir;
    dir.listDir("faces");
    for (int i = 0; i < dir.size(); i++) {
        ofImage photo;
        photos.push_back(photo);
        photos.back().load(dir.getPath(i));
        auto pixels = photos.back().getPixels();
        for (int x = 0; x < pixels.getWidth(); x++) {
            for (int y = 0; y < pixels.getHeight(); y++) {
                float b = pixels.getColor(x, y).getBrightness();
                ofColor c = ofColor(b);
                photos.back().setColor(x, y, c);
            }
        }
        photos.back().update();
        names.push_back(dir.getName(i));
    }

    tracker.setup();
    videoTracker.setup();
    grabber.setDeviceID(1);
    grabber.setup(640, 480);

    slep.set(0, 0);
    srep.set(0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {
//    idx = ofMap(mouseX, 0, ofGetWidth(), 0, photos.size() - 1, true);
    grabber.update();
    if (grabber.isFrameNew()) {
        tracker.update(photos[idx]);
        videoTracker.update(grabber);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {

//    ofPushMatrix();
//    grabber.draw(500, 0);
//    ofTranslate(500, 0);
//    videoTracker.drawDebug();
//    videoTracker.drawDebugPose();
//    ofPopMatrix();
//    photos[idx].draw(0, 50);
//    ofSetColor(ofColor::white);
//    ofDrawBitmapString(names[idx], 0, 20);
//    ofTranslate(0, 50);
//    tracker.drawDebug();
//    tracker.drawDebugPose();

    if (videoTracker.size() > 0 && tracker.size() > 0) {
        ofPoint lep = getCenterOfFeature(videoTracker, ofxFaceTracker2Landmarks::LEFT_EYE);
        ofPoint rep = getCenterOfFeature(videoTracker, ofxFaceTracker2Landmarks::RIGHT_EYE);
        slep = slep * 0.9 + lep * 0.1;
        srep = srep * 0.9 + rep * 0.1;

        ofPoint lep2 = getCenterOfFeature(tracker, ofxFaceTracker2Landmarks::LEFT_EYE);
        ofPoint rep2 = getCenterOfFeature(tracker, ofxFaceTracker2Landmarks::RIGHT_EYE);

        ofImage img;
        img.allocate(grabber.getWidth(), grabber.getHeight(), OF_IMAGE_GRAYSCALE);
        auto pixels =grabber.getPixels();
        for (int x = 0; x < grabber.getWidth(); x++) {
            for (int y = 0; y < grabber.getHeight(); y++) {
                float b = pixels.getColor(x, y).getBrightness();
                ofColor c = ofColor(b);
                img.setColor(x, y, c);
            }
        }
        img.update();

        img.setAnchorPoint(slep.x, slep.y);
        photos[idx].setAnchorPoint(lep2.x, lep2.y);

        ofPushMatrix();
        ofSetColor(255, 255, 255, 255);
        float dist = (slep - srep).length();
        ofPoint diff = srep - slep;
        float angle = atan2(diff.y, diff.x);
        float scale = 200. / dist;
        ofTranslate(300, 300);
        ofScale(scale, scale);
        ofRotateZRad(-angle);
        img.draw(0, 0);
        ofPopMatrix();

        ofPushMatrix();
        ofSetColor(255, 255, 255, ofMap(sin(ofGetElapsedTimef() * 7), -1, 1, 255 * 0, 255 * 0.8, true));
        dist = (lep2 -rep2).length();
        diff = rep2 - lep2;
        angle = atan2(diff.y, diff.x);
        scale = 200. / dist;
        ofTranslate(300, 300);
        ofScale(scale, scale);
        ofRotateZRad(-angle);
        photos[idx].draw(0, 0);
        ofPopMatrix();

        if (ofMap(sin(ofGetElapsedTimef() * 7), -1, 1, 255 * 0, 255 * 0.75, true) < 5) {
            idx = (int)ofRandom(0, photos.size() - 1);
        }
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
void ofApp::mouseReleased(int x, int y, int button) {
    idx = (idx + 1) % photos.size();
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

#include "ofApp.h"

FaceMeasures measureFace(ofxFaceTracker2 &tracker) {
    auto lmks = tracker.getInstances()[0].getLandmarks();

    auto leyf = lmks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
    auto reyf = lmks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
    auto lebf = lmks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYEBROW);
    auto rebf = lmks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYEBROW);
    auto nosf = lmks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BASE);
    auto mthf = lmks.getImageFeature(ofxFaceTracker2Landmarks::OUTER_MOUTH);
    auto ljwf = lmks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_JAW);
    auto rjwf = lmks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_JAW);
    auto nobf = lmks.getImageFeature(ofxFaceTracker2Landmarks::NOSE_BRIDGE);

    // Distance between the center of the eyes.
    ofPoint leyc = leyf.getCentroid2D();
    ofPoint reyc = reyf.getCentroid2D();
    float eyes = (reyc - leyc).length();

    // Distance between the left and right eyebrows.
    ofPoint eblp = lebf.getVertices()[0];
    auto ebvs = rebf.getVertices();
    ofPoint ebrp = ebvs[ebvs.size() - 1];
    float eyebrows = (ebrp - eblp).length();

    // Distance between the sides of the nose base.
    auto nosv = nosf.getVertices();
    ofPoint nosl = nosv[0];
    ofPoint nosr = nosv[nosv.size() - 1];
    float noseBase = (nosr - nosl).length();

    // Distance between the sides of the mouth.
    auto mthv = mthf.getVertices();
    ofPoint mthl = mthv[0];
    ofPoint mthr = mthv[mthv.size() / 2.0];
    float mouth = (mthr - mthl).length();

    // Distance between the sides of the jaw tops.
    ofPoint ljwp = ljwf.getVertices()[0];
    auto rjwv = rjwf.getVertices();
    ofPoint rjwp = rjwv[rjwv.size() - 1];
    float faceWidth = (rjwp - ljwp).length();

    // Distance between the top center and the bottom of the jaw.
    ofSetColor(ofColor::green);
    ofPoint tjwp = (ljwp + rjwp) / 2.0;
    ofPoint bjwp = rjwv[0];
    float faceHeight = (bjwp - tjwp).length();

    // Distance between the top of the bridge and the bottom of the nose.
    ofSetColor(ofColor::orange);
    ofPoint tnsp = nobf.getVertices()[0];
    ofPoint bnsp = nosv[nosv.size() / 2.0];
    float noseBridge = (bnsp - tnsp).length();

    // Distance between the bottom of the nose and the mouth.
    ofPoint tnmp = bnsp;
    ofPoint bnmp = mthf.getCentroid2D();
    float lip = (tnmp - bnmp).length();

    // Distance between the mouth and the jaw.
    auto tnjp = bnmp;
    auto bnjp = bjwp;
    float jaw = (tnjp - bnjp).length();

    // Normalize values and return.
    FaceMeasures measures;
    measures.eyes = eyes / faceWidth;
    measures.eyebrows = eyebrows / faceWidth;
    measures.noseBase = noseBase / faceWidth;
    measures.mouth = mouth / faceWidth;
    measures.noseBridge = noseBridge / faceHeight;
    measures.lip = lip / faceHeight;
    measures.jaw = jaw / faceHeight;

    return measures;
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
    //    videoTracker.setup();
    //    grabber.setDeviceID(1);
    //    grabber.setup(640, 480);

    slep.set(0, 0);
    srep.set(0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {
    idx = ofMap(mouseX, 0, ofGetWidth(), 0, photos.size() - 1, true);
    tracker.update(photos[idx]);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);
    photos[idx].draw(0, 0);

    if (tracker.size() == 0) return;

    ofSetLineWidth(2);
    tracker.drawDebug();
    ofSetColor(ofColor::cyan);
    ofSetLineWidth(4);

    auto m = measureFace(tracker);

    // Debug
    ofSetColor(ofColor::white);
    ofDrawBitmapString(
       "eyes: " + ofToString(m.eyes, 5) + "\n" +
       "eyebrows: " + ofToString(m.eyebrows, 5) + "\n" +
       "noseBase: " + ofToString(m.noseBase, 5) + "\n" +
       "mouth: " + ofToString(m.mouth, 5) + "\n" +
       "noseBridge: " + ofToString(m.noseBridge, 5) + "\n" +
       "lip: " + ofToString(m.lip, 5) + "\n" +
       "jaw: " + ofToString(m.jaw, 5),
        40, 40);
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

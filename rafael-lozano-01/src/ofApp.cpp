#include "ofApp.h"

//--------------------------------------------------------------
FaceMeasures ofApp::measureFace(ofxFaceTracker2 &tracker) {
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
    ofPoint tjwp = (ljwp + rjwp) / 2.0;
    ofPoint bjwp = rjwv[0];
    float faceHeight = (bjwp - tjwp).length();

    // Distance between the top of the bridge and the bottom of the nose.
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
    measures.faceProp = faceWidth / faceHeight;

    return measures;
}

//--------------------------------------------------------------
int ofApp::compareWithFaces(ofxFaceTracker2 &tracker) {
    auto m = measureFace(tracker);

    float max = 0;
    float maxIdx = -1;

    for (int i = 0; i < measures.size(); i++) {
        auto mi = measures[i];
        float d1 = 1.0 - ofDist(m.eyes, 0, mi.eyes, 0);
        float d2 = 1.0 - ofDist(m.eyebrows, 0, mi.eyebrows, 0);
        float d3 = 1.0 - ofDist(m.noseBase, 0, mi.noseBase, 0);
        float d4 = 1.0 - ofDist(m.noseBridge, 0, mi.noseBridge, 0);
        float d5 = 1.0 - ofDist(m.mouth, 0, mi.mouth, 0);
        float d6 = 1.0 - ofDist(m.lip, 0, mi.lip, 0);
        float d7 = 1.0 - ofDist(m.jaw, 0, mi.jaw, 0);
        float d8 = 1.0 - ofDist(m.faceProp, 0, mi.faceProp, 0);
        float conf = d1 * d2 * d3 * d4 * d5 * d6 * d7;
        if (conf > max) {
            max = conf;
            maxIdx = i;
        }
    }

    idx = maxIdx;
}

//--------------------------------------------------------------
void ofApp::setup() {
    tracker.setup();
    tracker.setThreaded(false);

    ofDirectory dir;
    dir.listDir("photos");
    for (int i = 0; i < dir.size(); i++) {
        ofImage photo;
        photos.push_back(photo);
        photos.back().load(dir.getPath(i));

        do {
            tracker.update(photos.back());
        } while (tracker.size() == 0);
        measures.push_back(measureFace(tracker));

        string name = dir.getName(i);
        auto fileNameParts = ofSplitString(name, ".");
        auto nameParts = ofSplitString(fileNameParts[0], ", ");
        StudentName studentName;
        studentName.lastName = nameParts[0];
        studentName.firstName = nameParts[1];
        names.push_back(studentName);

        cout << nameParts[1] << " " << nameParts[0] << endl;
    }
    cout << names.size() << " students loaded." << endl;

    tracker.setThreaded(true);
    grabber.setDeviceID(1);
    grabber.setup(640, 480);
}

//--------------------------------------------------------------
void ofApp::update() {
//    idx = ofMap(mouseX, 0, ofGetWidth(), 0, photos.size() - 1, true);

    grabber.update();
    if (grabber.isFrameNew()) {
        tracker.update(grabber);

        if (tracker.size() > 0) {
            compareWithFaces(tracker);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);
    grabber.draw(300, 160);
    photos[idx].draw(0, 0);

    // Debug
    auto m = measures[idx];
    ofSetColor(ofColor::white);
    ofDrawBitmapString(
        ofToUpper(names[idx].lastName) + ", " + names[idx].firstName + "\n" +
            "eyes: " + ofToString(m.eyes, 5) + "\n" +
            "eyebrows: " + ofToString(m.eyebrows, 5) + "\n" +
            "noseBase: " + ofToString(m.noseBase, 5) + "\n" +
            "mouth: " + ofToString(m.mouth, 5) + "\n" +
            "noseBridge: " + ofToString(m.noseBridge, 5) + "\n" + "lip: " +
            ofToString(m.lip, 5) + "\n" + "jaw: " + ofToString(m.jaw, 5),
        40, 40);

    if (tracker.size() > 0) {
        m = measureFace(tracker);
        ofSetColor(ofColor::white);
        ofDrawBitmapString("eyes: " + ofToString(m.eyes, 5) + "\n" +
                               "eyebrows: " + ofToString(m.eyebrows, 5) + "\n" +
                               "noseBase: " + ofToString(m.noseBase, 5) + "\n" +
                               "mouth: " + ofToString(m.mouth, 5) + "\n" +
                               "noseBridge: " + ofToString(m.noseBridge, 5) +
                               "\n" + "lip: " + ofToString(m.lip, 5) + "\n" +
                               "jaw: " + ofToString(m.jaw, 5),
                           340, 40);
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

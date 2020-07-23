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

    // Get the average eye width.
    auto leyv = leyf.getVertices();
    ofPoint llep = leyv[0];
    ofPoint lrep = leyv[leyv.size() / 2];
    auto reyv = reyf.getVertices();
    ofPoint rlep = reyv[0];
    ofPoint rrep = reyv[reyv.size() / 2];
    float eyeWidth = ((lrep - llep).length() + (rrep - rlep).length()) / 2;

    // Get the average eyebrow width.
    auto lebv = lebf.getVertices();
    llep = lebv[0];
    lrep = lebv[lebv.size() - 1];
    auto rebv = rebf.getVertices();
    rlep = rebv[0];
    rrep = rebv[rebv.size() - 1];
    float eyebrowWidth = ((lrep - llep).length() + (rrep - rlep).length()) / 2;

    // Distance between the center of the eyes.
    ofPoint leyc = leyf.getCentroid2D();
    ofPoint reyc = reyf.getCentroid2D();
    float eyes = (reyc - leyc).length();

    // Distance between the left and right eyebrows.
    ofPoint eblp = lebv[0];
    ofPoint ebrp = rebv[rebv.size() - 1];
    float eyebrows = (ebrp - eblp).length();

    // Distance between the sides of the nose base.
    auto nosv = nosf.getVertices();
    ofPoint nosl = nosv[0];
    ofPoint nosr = nosv[nosv.size() - 1];
    float noseBase = (nosr - nosl).length();

    // Distance between the sides of the mouth.
    auto mthv = mthf.getVertices();
    ofPoint mthl = mthv[0];
    ofPoint mthr = mthv[mthv.size() / 2];
    float mouth = (mthr - mthl).length();

    // Distance between the sides of the jaw tops.
    ofPoint ljwp = ljwf.getVertices()[0];
    auto rjwv = rjwf.getVertices();
    ofPoint rjwp = rjwv[rjwv.size() - 1];
    float faceWidth = (rjwp - ljwp).length();

    // Distance between the top center and the bottom of the jaw.
    ofPoint tjwp = (ljwp + rjwp) / 2;
    ofPoint bjwp = rjwv[0];
    float faceHeight = (bjwp - tjwp).length();

    // Distance between the top of the bridge and the bottom of the nose.
    ofPoint tnsp = nobf.getVertices()[0];
    ofPoint bnsp = nosv[nosv.size() / 2];
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
    measures.eyeWidth = eyeWidth / faceWidth;
    measures.eyebrowWidth = eyebrowWidth / faceWidth;
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
        float d0 = 1.0 - ofDist(m.eyes, 0, mi.eyes, 0);
        float d1 = 1.0 - ofDist(m.eyebrows, 0, mi.eyebrows, 0);
        float d2 = 1.0 - ofDist(m.noseBase, 0, mi.noseBase, 0);
        float d3 = 1.0 - ofDist(m.noseBridge, 0, mi.noseBridge, 0);
        float d4 = 1.0 - ofDist(m.mouth, 0, mi.mouth, 0);
        float d5 = 1.0 - ofDist(m.lip, 0, mi.lip, 0);
        float d6 = 1.0 - ofDist(m.jaw, 0, mi.jaw, 0);
        float d7 = 1.0 - ofDist(m.faceProp, 0, mi.faceProp, 0);
        float d8 = 1.0 - ofDist(m.eyeWidth, 0, mi.eyeWidth, 0);
        float d9 = 1.0 - ofDist(m.eyebrowWidth, 0, mi.eyebrowWidth, 0);
        float conf = d0 * d1 * d2 * d3 * d4 * d5 * d6 * d7 * d8 * d9;
        if (conf > max) {
            max = conf;
            maxIdx = i;
        }
    }

    confidence = max * 100.0;
    previousPhoto = currentPhoto;
    currentPhoto = maxIdx;
}

//--------------------------------------------------------------
void ofApp::setup() {
    ofEnableAntiAliasing();
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    ofSetFrameRate(FRAME_RATE);

    tracker.setThreaded(false);
    tracker.setup();

    ofDirectory dir;
    dir.listDir("photos");
    for (int i = 0; i < dir.size(); i++) {
        ofImage photo;
        photos.push_back(photo);
        photos.back().load(dir.getPath(i));

        tracker.update(photos.back());
        measures.push_back(measureFace(tracker));

        EyesLocation eyesLocation;
        eyes.push_back(eyesLocation);
        auto lmks = tracker.getInstances()[0].getLandmarks();
        auto lEye = lmks.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
        ofPoint lEyeP = lEye.getCentroid2D();
        auto rEye = lmks.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
        ofPoint rEyeP = rEye.getCentroid2D();
        eyes.back().left = lEyeP;
        eyes.back().right = rEyeP;

        string name = dir.getName(i);
        auto fileNameParts = ofSplitString(name, ".");
        auto nameParts = ofSplitString(fileNameParts[0], ", ");
        StudentName studentName;
        studentName.lastName = nameParts[0];
        studentName.firstName = nameParts[1];
        names.push_back(studentName);

        ofImage img;
        images.push_back(img);

        cout << nameParts[1] << " " << nameParts[0] << endl;
    }
    cout << names.size() << " students loaded." << endl;

    currentPhoto = 0;
    previousPhoto = 0;
    confidence = 0.0;

    ofTrueTypeFontSettings settings("Lato-Bold.ttf", 40);
    settings.antialiased = true;
    font1.load(settings);

    settings = ofTrueTypeFontSettings("Lato-Regular.ttf", 24);
    settings.antialiased = true;
    font2.load(settings);

    shader.load("", "shader.frag");
    tracker.setThreaded(true);
    tracker.setup();
    grabber.setDeviceID(1);
    grabber.setup(640, 480);
    slep.set(0, 0);
    srep.set(0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (ofGetFrameNum() % (int)(FRAME_RATE * 3.0) == 0) {
        shader.load("", "shader.frag");
    }

    grabber.update();
    if (grabber.isFrameNew()) {
        
        tracker.update(grabber);
        if (tracker.size() == 1) {

            auto lmk = tracker.getInstances()[0].getLandmarks();
            auto lef = lmk.getImageFeature(ofxFaceTracker2Landmarks::LEFT_EYE);
            auto ref = lmk.getImageFeature(ofxFaceTracker2Landmarks::RIGHT_EYE);
            ofPoint lep = lef.getCentroid2D();
            ofPoint rep = ref.getCentroid2D();
            slep = slep * 0.9 + lep * 0.1;
            srep = srep * 0.9 + rep * 0.1;

            ofFbo fbo;
            fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
            fbo.begin();
            ofClear(170, 170, 170, 0);
            cameraImage.setFromPixels(grabber.getPixels());
            for (int x = 0; x < cameraImage.getWidth(); x++) {
                for (int y = 0 ; y < cameraImage.getHeight(); y++) {
                    float b = cameraImage.getColor(x,y).getBrightness();
                    cameraImage.setColor(x, y, ofColor(b));
                }
            }
            cameraImage.update();
            cameraImage.setAnchorPoint(lep.x, lep.y);
            float dist = (slep - srep).length();
            ofPoint diff = srep - slep;
            float angle = atan2(diff.y, diff.x);
            float scale = 150.0 / dist;
            ofTranslate(310, 390);
            ofScale(scale, scale);
            ofRotateZDeg(-angle);
            ofSetColor(ofColor::white);
            cameraImage.draw(0, 0);
            fbo.end();

            ofPixels pixels;
            fbo.readToPixels(pixels);
            cameraImage.setAnchorPoint(0, 0);
            cameraImage.setFromPixels(pixels);
            cameraImage.update();

            if (ofGetElapsedTimef() - lastCheck > 3.0) {
                lastCheck = ofGetElapsedTimef();
                compareWithFaces(tracker);
            }

            return;
        }
    }

    if (confidence > 0 && tracker.size() != 1) {
        lastCheck = 0.0;
        confidence = 0.0;
    }

    if (confidence == 0.0 && ofGetFrameNum() % (int)(FRAME_RATE * PHOTO_DELAY) == 0) {
        previousPhoto = currentPhoto;
        currentPhoto = (currentPhoto + 1) % photos.size();
        prepareFbo(currentPhoto);
    }
}

//--------------------------------------------------------------
void ofApp::prepareFbo(int fboIdx) {
    if (images[fboIdx].isAllocated()) return;

    ofFbo fbo;
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    fbo.begin();
    ofClear(170, 170, 170, 0);
    auto eyesLocation = eyes[fboIdx];
    ofImage photo = photos[fboIdx];
    for (int x = 0; x < photo.getWidth(); x++) {
        for (int y = 0; y < photo.getHeight(); y++) {
            float b = photo.getColor(x, y).getBrightness();
            photo.setColor(x, y, ofColor(b));
        }
    }
    photo.update();
    photo.setAnchorPoint(eyesLocation.left.x, eyesLocation.left.y);
    float dist = (eyesLocation.left - eyesLocation.right).length();
    ofPoint diff = eyesLocation.right - eyesLocation.left;
    float angle = atan2(diff.y, diff.x);
    float scale = 150.0 / dist;
    ofTranslate(310, 390);
    ofScale(scale, scale);
    ofRotateZRad(-angle);
    ofSetColor(ofColor::white);
    photo.draw(0, 0);
    fbo.end();

    ofPixels pixels;
    fbo.readToPixels(pixels);
    images[fboIdx].setFromPixels(pixels);
    images[fboIdx].update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(170, 170, 170);

    if (confidence > 0.0) {
        ofSetColor(ofColor::white);
        cameraImage.draw(0,0);
//        tracker.drawDebug(0, 0);
    } else {
        int framesThreshold = FRAME_RATE * PHOTO_DELAY;
        int relativeFrameNum = ofGetFrameNum() % framesThreshold;
        float alpha = ofMap(relativeFrameNum, 0, framesThreshold, 0, 255, true);

        ofSetColor(255, 255, 255, 255 - alpha);
        prepareFbo(previousPhoto);
        images[previousPhoto].draw(0, 0);

        ofSetColor(255, 255, 255, alpha);
        prepareFbo(currentPhoto);
        images[currentPhoto].draw(0, 0);
    }

    ofSetColor(50, 50, 50);
    auto name = names[currentPhoto];
    auto box = font1.getStringBoundingBox(name.firstName, 0, 0);
    float y = 790;
    font1.drawString(name.firstName, (ofGetWidth() - box.getWidth()) / 2.0, y);
    box = font1.getStringBoundingBox(name.lastName, 0, 0);
    y += font1.getLineHeight() * 0.9;
    font1.drawString(name.lastName, (ofGetWidth() - box.getWidth()) / 2.0, y);

    y += font1.getLineHeight() * 0.9;
    string status = "Estudiante no encontrado";
    box = font2.getStringBoundingBox(status, 0, 0);
    font2.drawString(status, (ofGetWidth() - box.getWidth()) / 2.0, y);

    y += font2.getLineHeight() * 1.1;
    string result = "Nivel de confianza: " + ofToString(confidence, 2) + "%";
    box = font2.getStringBoundingBox(result, 0, 0);
    font2.drawString(result, (ofGetWidth() - box.getWidth()) / 2.0, y);

    //    shader.begin();
    //    shader.setUniform1f("time", ofGetElapsedTimef());
    //    //    shader.setUniform2f("mouse", mouseX, mouseY);
    //    //    shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
    //    shader.setUniformTexture("img0", images[drawQueue[0]], 0);
    //    shader.setUniformTexture("img1", images[drawQueue[1]], 1);
    ////    shader.setUniformTexture("img2", images[drawQueue[2]], 2);
    ////    shader.setUniformTexture("img3", images[drawQueue[3]], 3);
    ////    shader.setUniformTexture("img4", images[drawQueue[4]], 4);
    //    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    //    shader.end();

    //    ofSetColor(ofColor::black);
    //    ofDrawBitmapString(ofToString(ofGetFrameRate()) + "\n" + "alpha: " +
    //    ofToString(alpha, 2), 40, 40);
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

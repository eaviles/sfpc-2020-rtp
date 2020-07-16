#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(IMAGE_WIDTH * 2, IMAGE_HEIGHT * 2);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    player.load("input.mov");
    player.setSpeed(1.5);
    player.play();
    player.setLoopState(OF_LOOP_NORMAL);

    grayImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    diffImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    prevImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    thrsImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_GRAYSCALE);
    bodyImg.allocate(IMAGE_WIDTH, IMAGE_HEIGHT, OF_IMAGE_COLOR_ALPHA);

    palette.push_back(ofColor::magenta);
    palette.push_back(ofColor::red);
    palette.push_back(ofColor::yellow);
    palette.push_back(ofColor::green);
    palette.push_back(ofColor::blue);
    palette.push_back(ofColor::white);

    motionFinder.setMinArea((float)IMAGE_WIDTH * 0.25);
    bodyFinder.setMinArea((float)IMAGE_WIDTH * 0.25);

    for (int i = 0; i < maxFieldLines; i++) {
        fieldLines.push_back(fieldLine());
    }
}

//--------------------------------------------------------------
void ofApp::update() {
    player.update();
    if (!player.isFrameNew()) return;
    //    tracker.update(player);

    // Get a greyscale image from the camera.
    convertColor(player, grayImg, CV_RGB2GRAY);
    grayImg.resize(IMAGE_WIDTH, IMAGE_HEIGHT);
    grayImg.update();

    // Get the diff to detect motion.
    absdiff(grayImg, prevImg, diffImg);
    diffImg.update();

    // Threshold the diff image.
    thrsImg.setFromPixels(diffImg.getPixels());
    threshold(thrsImg, 256 * 0.25);
    thrsImg.update();

    int numMovPixels = 0;
    for (int x = 0; x < IMAGE_WIDTH; x++) {
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
            numMovPixels += thrsImg.getColor(x, y).r > 127;
        }
    }
    smoothNumMovPixels = 0.9 * smoothNumMovPixels + 0.1 * numMovPixels;
    maxFieldLines = ofMap(smoothNumMovPixels, 0, 16000, 512, 12000, true);

    // Store the prev image.
    prevImg.setFromPixels(grayImg.getPixels());
    prevImg.update();

    // Find the motion contours.
    motionFinder.findContours(thrsImg);

    // Convert outlines to shapes.
    ofPath p;
    for (int i = 0; i < motionFinder.size(); i++) {
        auto pl = motionFinder.getPolyline(i).getResampledByCount(24);
        for (int j = 0; j < pl.getVertices().size(); j++) {
            auto v = pl.getVertices()[j];
            float rx = 0;
            float ry = 0;
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

    // Maintain a history of motion shapes.
    history.push_back(p);
    if (history.size() > palette.size()) {
        history.erase(history.begin());
    }

    // Threshold the camera image to get the body.
    thrsImg.setFromPixels(grayImg.getPixels());
    float bodyThreshold = 256 * 0.65;
    thrsImg.update();

    bodyFinder.findContours(thrsImg);

    auto pxls = grayImg.getPixels();
    for (int x = 0; x < IMAGE_WIDTH; x++) {
        for (int y = 0; y < IMAGE_HEIGHT; y++) {
            auto c = pxls.getColor(x, y);
            auto d = ofColor(0, 0, 0, c.r < bodyThreshold ? 0 : 255);
            bodyImg.setColor(x, y, d);
        }
    }
    bodyImg.update();
}

//------------------------------------------------------------------------------
void ofApp::drawBackground() {
    while (fieldLines.size() > maxFieldLines) {
        fieldLines.erase(fieldLines.end() - 1);
    }
    while (fieldLines.size() < maxFieldLines) {
        fieldLines.push_back(fieldLine());
    }

    for (int i = 0; i < fieldLines.size(); i++) {
        fieldLine l = fieldLines[i];
        if (l.z - l.l > -FIELD_LINES_DEPTH) {
            ofSetLineWidth(l.w);
            ofDrawLine(l.x, l.y, l.z, l.x, l.y, l.z - l.l);
            fieldLines[i].z = l.z - FIELD_LINES_SPEED;
        } else {
            fieldLines[i] = fieldLine();
        }
    }
}

//------------------------------------------------------------------------------
void ofApp::drawMotion() {
    for (int i = 0; i < history.size(); i++) {
        history[i].setColor(palette[i]);
        history[i].draw();
    }
}

//------------------------------------------------------------------------------
void ofApp::drawBody() {
    if (bodyFinder.size() == 0) return;

    for (int i = 0; i < bodyFinder.size(); i++) {
        auto d = bodyFinder.getPolyline(i).getResampledBySpacing(6);
        ofSetColor(ofColor::white);
        ofSetLineWidth(6);
        d.draw();

        vector<ofPoint> pts;
        for (int j = 0; j < d.size(); j++) {
            pts.push_back(d[j]);
        }
        ofRandomize(pts);

        for (int j = 1; j < pts.size(); j++) {
            ofPoint p1 = pts[j - 1];
            ofPoint p2 = pts[j];
            auto d = (p1 - p2).length();
            if (d > 80) continue;
            ofSetLineWidth(d < 40 ? 6 : 2);
            ofDrawLine(p1, p2);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::black);
    ofSetColor(ofColor::white);
    ofScale(2.0);

    drawBackground();

    ofTranslate((float)IMAGE_WIDTH * 0.1, (float)IMAGE_HEIGHT * 0.1,
                (float)IMAGE_HEIGHT * -0.2);
    ofScale(0.8);

    ofPushMatrix();
    ofTranslate((float)IMAGE_WIDTH * -0.25, 0, (float)IMAGE_WIDTH * -0.2);
    drawMotion();
    bodyImg.draw(0, 0);
    drawBody();
    ofPopMatrix();

    ofPushMatrix();
    ofTranslate((float)IMAGE_WIDTH * 0.25, 0, (float)IMAGE_WIDTH * -0.2);
    drawMotion();
    bodyImg.draw(0, 0);
    drawBody();
    ofPopMatrix();

    ofPushMatrix();
    drawMotion();
    bodyImg.draw(0, 0);
    drawBody();
    ofPopMatrix();
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

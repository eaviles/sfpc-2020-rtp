#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(720, 1280);
    ofSetFrameRate(24);
    ofSetLineWidth(6.0);

    ofDirectory dir;
    dir.listDir("images");
    dir.sort();

    for (int i = 0; i < dir.size(); i++) {
        string filePath = dir.getPath(i);
        auto pathParts = ofSplitString(filePath, "/");
        int prcLoad = round((float)i / dir.size() * 100.0);
        cout << "Loading " << pathParts[1] << " (" << prcLoad << "%)" << endl;

        string maskFilePath = "masks/" + pathParts[1];
        ofImage img;
        images.push_back(img);
        images.back().load(filePath);
        auto imgPixels = images.back().getPixels();

        ofImage maskImg;
        masks.push_back(maskImg);
        masks.back().load(maskFilePath);

        auto maskPixels = masks.back().getPixels();
        ofImage strImg;
        strImg.allocate(imgPixels.getWidth(), imgPixels.getHeight(),
                        OF_IMAGE_COLOR);
        vector<int> imgEdges;
        for (int y = 0; y < maskPixels.getHeight(); y++) {
            int fromLeft = 0;
            for (int x = 0; x < maskPixels.getWidth(); x++) {
                int brightness = maskPixels.getColor(x, y).getBrightness();
                if (brightness < 255) {
                    fromLeft = x;
                    break;
                }
            }
            int fromRight = maskPixels.getWidth();
            for (int x = maskPixels.getWidth() - 1; x >= 0; x--) {
                int brightness = maskPixels.getColor(x, y).getBrightness();
                if (brightness > 0) {
                    fromRight = x;
                    break;
                }
            }
            int edge = round((float)(fromLeft + fromRight) / 2.0);
            imgEdges.push_back(edge);

            for (int x = 0; x < imgPixels.getWidth(); x++) {
                int offset = round(((float)imgPixels.getWidth() * 0.55) - edge);
                int safeX = ofClamp(x - offset, 0, imgPixels.getWidth() - 1);
                ofColor c = imgPixels.getColor(safeX, y);
                strImg.setColor(x, y, c);
            }
        }
        edges.push_back(imgEdges);
        strImg.update();
        correctedImages.push_back(strImg);
    }

    cout << "Calculating average image…" << endl;
    averageImage.allocate(ofGetWidth(), ofGetHeight(), OF_IMAGE_COLOR);
    for (int x = 0; x < ofGetWidth(); x++) {
        for (int y = 0; y < ofGetHeight(); y++) {
            float avgR = 0.0;
            float avgG = 0.0;
            float avgB = 0.0;
            for (int i = 0; i < images.size(); i++) {
                ofColor c = images[i].getColor(x, y);
                avgR += c.r;
                avgG += c.g;
                avgB += c.b;
            }
            avgR /= (float)images.size();
            avgG /= (float)images.size();
            avgB /= (float)images.size();
            averageImage.setColor(x, y, ofColor(avgR, avgG, avgB));
        }
    }
    averageImage.update();
    cout << "Average image ready." << endl;
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetColor(ofColor::white);

    int width = ofGetWidth();
    int imgsCount = images.size();

    // Get the current image index for auto-play or for the mouse position.
    int imgIdx = 0;
    if (!mouseIsDown) {
        imgIdx = ofGetFrameNum() % (imgsCount * 2);
        if (imgIdx > imgsCount - 1) {
            imgIdx = imgsCount * 2 - imgIdx - 1;
        }
    } else {
        imgIdx = ofMap(mouseX, 0, width, 0, imgsCount - 1, true);
    }

    if (drawOriginal) {
        images[imgIdx].draw(0, 0);

        if (drawMaskEdges) {
            ofEnableAlphaBlending();
            ofSetColor(255, 255, 255, 128);
            masks[imgIdx].draw(0, 0);
            ofDisableAlphaBlending();

            ofSetColor(ofColor::cyan);
            auto imgEdges = edges[imgIdx];
            for (int i = 1; i < imgEdges.size(); i++) {
                ofDrawLine(imgEdges[i - 1], i - 1, imgEdges[i], i);
            }
        }
    }

    if (drawStraightened) {
        correctedImages[imgIdx].draw(0, 0);
    }

    if (drawAverage) {
        averageImage.draw(0, 0);
    }

    if (drawAligned) {
        if (drawStraightened) {
            ofEnableAlphaBlending();
            ofSetColor(255, 255, 255, 128);
        } else
            averageImage.draw(0, 0);

        int imgWidth = images[imgIdx].getWidth();
        int imgHeight = images[imgIdx].getHeight();
        auto imgEdges = edges[imgIdx];
        float avgEdge = 0.0;
        float topAvg = 0.0;
        float bottomAvg = 0.0;
        for (int i = 0; i < imgEdges.size(); i++) {
            if (i < imgEdges.size() / 2) {
                topAvg += imgEdges[i];
            } else {
                bottomAvg += imgEdges[i];
            }
            avgEdge += imgEdges[i];
        }
        topAvg /= (float)imgEdges.size() / 2.0;
        bottomAvg /= (float)imgEdges.size() / 2.0;
        avgEdge /= (float)imgEdges.size();

        float distance = ofDist(0, imgHeight / 2.0, imgEdges[imgHeight / 2],
                                imgHeight / 2.0);
        float scale = ((float)imgWidth * 0.55) / distance;
        if (!smoothInitiated) smoothScale = scale;
        smoothScale = 0.8 * smoothScale + 0.2 * scale;

        ofPoint diff = ofPoint(bottomAvg, imgHeight - 1) - ofPoint(topAvg, 0);
        float angle = atan2(diff.y, diff.x);
        if (!smoothInitiated) smoothAngle = angle;
        smoothAngle = 0.8 * smoothAngle + 0.2 * angle;

        if (!smoothInitiated) smoothInitiated = true;

        ofPushMatrix();
        ofTranslate(0, 0);
        ofScale(smoothScale, smoothScale);
        ofRotateZRad(-smoothAngle);
        ofRotateZDeg(90);
        images[imgIdx].draw(0, 0);
        ofPopMatrix();

        if (drawStraightened) {
            ofDisableAlphaBlending();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    switch (key) {
        case '2':
            drawOriginal = true;
            drawMaskEdges = true;
            drawStraightened = false;
            drawAligned = false;
            drawAverage = false;
            break;
        case '3':
            drawOriginal = false;
            drawMaskEdges = false;
            drawStraightened = true;
            drawAligned = false;
            drawAverage = false;
            break;
        case '4':
            drawOriginal = false;
            drawMaskEdges = false;
            drawStraightened = false;
            drawAligned = true;
            drawAverage = false;
            break;
        case '5':
            drawOriginal = false;
            drawMaskEdges = false;
            drawStraightened = false;
            drawAligned = false;
            drawAverage = true;
            break;
        case '6':
            drawOriginal = false;
            drawMaskEdges = false;
            drawStraightened = true;
            drawAligned = true;
            drawAverage = false;
            break;
        default:
            drawOriginal = true;
            drawMaskEdges = false;
            drawStraightened = false;
            drawAligned = false;
            drawAverage = false;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) { mouseIsDown = true; }

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) { mouseIsDown = false; }

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

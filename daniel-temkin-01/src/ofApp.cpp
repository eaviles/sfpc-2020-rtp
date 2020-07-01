#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetWindowShape(720, 1280);
    ofSetFrameRate(24);
    ofSetLineWidth(3.0);

    ofDirectory dir;
    dir.listDir("images");
    dir.sort();
    for (int i = 0; i < dir.size(); i++) {
        string filePath = dir.getPath(i);
        int prcLoad = round((float)i / dir.size() * 100.0);
        cout << "Loading " << filePath << " (" << prcLoad << "%)" << endl;

        auto pathParts = ofSplitString(filePath, "/");
        string maskFilePath = "masks/" + pathParts[1];
        ofImage img;
        images.push_back(img);
        images.back().load(filePath);

        ofImage maskImg;
        masks.push_back(maskImg);
        masks.back().load(maskFilePath);

        auto maskPixels = masks.back().getPixels();
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
        }
        edges.push_back(imgEdges);
    }
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

    images[imgIdx].draw(0, 0);
    ofEnableAlphaBlending();
    ofSetColor(255, 255, 255, 128);
    masks[imgIdx].draw(0, 0);
    ofDisableAlphaBlending();

    ofSetColor(ofColor::red);
    auto imgEdges = edges[imgIdx];
    for (int i = 1; i < imgEdges.size(); i++) {
        ofDrawLine(imgEdges[i - 1], i - 1, imgEdges[i], i);
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

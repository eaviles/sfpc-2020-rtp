#include "ofApp.h"

bool hueCompare(ofColor a, ofColor b) { return a.getHue() < b.getHue(); }

void ofApp::cacheFragment(int i) {
    string inFile = "fragments/i" + ofToString(i) + ".jpg";
    string outFile = "cache/i" + ofToString(i) + ".png";
    ofImage inImg = ofImage(inFile);

    int inImgW = inImg.getWidth();
    int inImgH = inImg.getHeight();
    if (inImgW > MIN_FRG_SIZE || inImgH > MIN_FRG_SIZE) {
        float scale = (float)MIN_FRG_SIZE / (inImgW > inImgH ? inImgW : inImgH);
        inImg.resize(inImgW * scale, inImgH * scale);
        inImgW = inImg.getWidth();
        inImgH = inImg.getHeight();
    }

    ofxCvColorImage colorImg;
    colorImg.allocate(inImgW, inImgH);
    colorImg.setFromPixels(inImg.getPixels());

    ofxCvGrayscaleImage grayImg;
    grayImg.allocate(inImgW, inImgH);
    grayImg = colorImg;
    grayImg.blurGaussian(8.0);
    grayImg.threshold(210);
    grayImg.invert();

    ofxCvContourFinder contourFinder;
    contourFinder.findContours(grayImg, 100, inImgW * inImgH, 1, false);
    ofxCvBlob blob = contourFinder.blobs[0];

    ofFbo maskImage;
    int maskW = blob.boundingRect.getWidth();
    int maskH = blob.boundingRect.getHeight();
    int maskX = blob.boundingRect.getLeft();
    int maskY = blob.boundingRect.getTop();
    maskImage.allocate(maskW, maskH, GL_RGBA);
    maskImage.begin();
    ofSetColor(ofColor::black);
    ofDrawRectangle(0, 0, maskW, maskH);
    ofSetColor(ofColor::white);
    ofBeginShape();
    for (int i = 0; i < blob.nPts; i++) {
        ofVertex(blob.pts[i].x - maskX, blob.pts[i].y - maskY);
    }
    ofEndShape(true);
    maskImage.end();
    ofPixels maskPixels;
    maskImage.readToPixels(maskPixels);

    ofImage compImg;
    compImg.allocate(maskW, maskH, OF_IMAGE_COLOR_ALPHA);
    for (int x = 0; x < maskW; x++) {
        for (int y = 0; y < maskH; y++) {
            ofColor maskColor = maskPixels.getColor(x, y);
            int a = maskColor.getBrightness();
            ofColor c = inImg.getColor(x + maskX, y + maskY);
            compImg.setColor(x, y, ofColor(c.r, c.g, c.b, a));
        }
    }
    compImg.save(outFile);
}

void ofApp::colorFragment(ofImage img, int i) {
    float r = 0, g = 0, b = 0;
    float n = 0;
    for (int x = 0; x < img.getWidth(); x++) {
        for (int y = 0; y < img.getHeight(); y++) {
            ofColor c = img.getColor(x, y);
            if (c.a < 255) continue;
            r += c.r;
            g += c.g;
            b += c.b;
            n++;
        }
    }
    string sr = ofToString((int)round(r / n));
    string sg = ofToString((int)round(g / n));
    string sb = ofToString((int)round(b / n));
    ofBuffer buffer;
    buffer.append(sr + "," + sg + "," + sb);
    ofBufferToFile("cache/i" + ofToString(i) + ".txt", buffer);
}

int ofApp::indexOfFragmentByColor(ofColor c) {
    float minDist = 1e6;
    int result = 0;
    for (int i = 0; i < fragmentColors.size(); i++) {
        ofColor fc = fragmentColors[i];
        float dist = ofDist(c.r, c.g, c.b, fc.r, fc.g, fc.b);
        //        dist += ofRandom(-50.0, 50.0);
        //                dist += ofxGaussian() * 4.0;
        if (dist < minDist && fragmentCounters[i] < MAX_FRG_REUSE) {
            result = i;
            minDist = dist;
        }
    }
    fragmentCounters[result]++;
    return result;
}

//--------------------------------------------------------------
void ofApp::setup() {
    showOrg = false;
    targetImage.load("spectrum.png");
    ofSetWindowShape((int)targetImage.getWidth(), (int)targetImage.getHeight());

    for (int i = 0; i < TOTAL_FRAGMENTS; i++) {
        ofImage frg;
        string filePath = "cache/i" + to_string(i) + ".png";
        bool fileExists = ofFile::doesFileExist(filePath);
        if (!fileExists) cacheFragment(i);
        frg.load(filePath);

        filePath = "cache/i" + to_string(i) + ".txt";
        fileExists = ofFile::doesFileExist(filePath);
        if (!fileExists) colorFragment(frg, i);
        auto cs = ofSplitString(ofBufferFromFile(filePath), ",");
        int r = ofToInt(cs[0]);
        int g = ofToInt(cs[1]);
        int b = ofToInt(cs[2]);
        ofColor avgColor = ofColor(r, g, b);

        fragments.push_back(frg);
        fragmentColors.push_back(avgColor);
    }
}

float done = false;

//--------------------------------------------------------------
void ofApp::update() {
    for (int i = 0; i < fragments.size(); i++) {
        fragmentCounters[i] = 0;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::white);
    ofSetColor(ofColor::white);

    if (showOrg) {
        targetImage.draw(0, 0);
        return;
    }

    float totalSize = ofGetWidth();
    float skip = totalSize / FRAGMENTATION;
    float midSkip = skip / 2.0;

    for (int x = 0; x < totalSize - midSkip; x += skip) {
        for (int y = 0; y < totalSize - midSkip; y += skip) {
            float avgR = 0, avgG = 0, avgB = 0;
            for (int x1 = 0; x1 < skip; x1++) {
                for (int y1 = 0; y1 < skip; y1++) {
                    ofColor c1 = targetImage.getColor(x + x1, y + y1);
                    avgR += c1.r;
                    avgG += c1.g;
                    avgB += c1.b;
                }
            }
            avgR /= skip * skip;
            avgG /= skip * skip;
            avgB /= skip * skip;
            ofColor c = ofColor(avgR, avgG, avgB);

            int frgIdx = indexOfFragmentByColor(c);
            ofImage frg = fragments[frgIdx];
            float frgW = frg.getWidth();
            float frgH = frg.getHeight();
            float scale = skip / (frgW < frgH ? frgH : frgW);
            scale *= ofMap(c.getBrightness(), 0, 255, 1.2, 0.8);
            //            scale *= 3.0;

            ofPushMatrix();
            ofTranslate(x, y);
            ofTranslate(midSkip, midSkip);
            ofScale(scale);
            ofPushMatrix();
            frg.setAnchorPercent(0.5, 0.5);
            ofRotateDeg(c.getBrightness());
            frg.draw(0, 0);
            ofPopMatrix();
            ofPopMatrix();
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
void ofApp::mousePressed(int x, int y, int button) { showOrg = !showOrg; }

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

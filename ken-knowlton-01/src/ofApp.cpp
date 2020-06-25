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
    ofxColorQuantizer colorQtr;
    colorQtr.setNumColors(4);
    colorQtr.quantize(img.getPixels());
    vector<ofColor> colors = colorQtr.getColors();
    float r = 0, g = 0, b = 0;
    int n = colors.size();
    for (int j = 0; j < n; j++) {
        ofColor c = colors[j];
        r += c.r;
        g += c.g;
        b += c.b;
    }
    string sr = ofToString((int)round(r / n));
    string sg = ofToString((int)round(g / n));
    string sb = ofToString((int)round(b / n));
    ofBuffer buffer;
    buffer.append(sr + "," + sg + "," + sb);
    ofBufferToFile("cache/i" + ofToString(i) + ".txt", buffer);
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
    //
    //
    //
    //
    //
    //
    //
    //
    //        fragmentCounters[i] = 0;
    //
    //        ofImage image;
    //        string file = "fragments/i" + to_string(i) + ".jpg";
    //        image.load(file);
    //        float orgImgWidth = image.getWidth();
    //        image.resize(image.getWidth() * 0.2, image.getHeight() * 0.2);
    //
    //        float imageWidth = image.getWidth();
    //        float imageHeight = image.getHeight();
    //
    //        ofImage imageCopy;
    //        imageCopy.load(file);
    //        imageCopy.resize(imageCopy.getWidth() * 0.05,
    //        imageCopy.getHeight() * 0.05); ofxColorQuantizer colorQuantizer;
    //        colorQuantizer.setNumColors(4);
    //        colorQuantizer.quantize(imageCopy.getPixels());
    //        vector<ofColor> palette = colorQuantizer.getColors();
    //        ofSort(palette, hueCompare);
    //
    //        float max = 0.0;
    //        int maxIndex = -1;
    //        for (int j = 0; j < palette.size(); j++) {
    //            if (palette[j].getBrightness() > max) {
    //                max = palette[j].getBrightness();
    //                maxIndex = j;
    //            }
    //        }
    //        palette.erase(palette.begin() + maxIndex);
    //
    //        float avgR = 0;
    //        float avgG = 0;
    //        float avgB = 0;
    //        for (int j = 0; j < palette.size(); j++) {
    //            avgR += palette[j].r;
    //            avgG += palette[j].g;
    //            avgB += palette[j].b;
    //        }
    //        avgR /= palette.size();
    //        avgG /= palette.size();
    //        avgB /= palette.size();
    //        palette.clear();
    //        palette.push_back(ofColor(avgR, avgG, avgB));
    //
    //        ofColor tl = image.getColor(10, 10);
    //        ofColor tr = image.getColor(imageWidth - 10, 10);
    //        ofColor bl = image.getColor(10, imageHeight - 10);
    //        ofColor br = image.getColor(imageWidth - 10, imageHeight - 10);
    //        ofColor tc = ofColor((tl.r + tr.r + bl.r + br.r) / 4.0,
    //                             (tl.g + tr.g + bl.g + br.g) / 4.0,
    //                             (tl.b + tr.b + bl.b + br.b) / 4.0);
    //
    //        image.setImageType(OF_IMAGE_COLOR_ALPHA);
    //        for (int x = 0; x < imageWidth; x++) {
    //            for (int y = 0; y < imageHeight; y++) {
    //                ofColor c = image.getColor(x, y);
    //                if (ofDist(c.r, c.g, c.b, tc.r, tc.g, tc.b) < 32) {
    //                    image.setColor(x, y, ofColor(c.r, c.g, c.b, 0));
    //                } else {
    //                    image.setColor(x, y, ofColor(c.r, c.g, c.b, 255));
    //                }
    //            }
    //        }
    //        image.update();
    //
    //        fragmentColors.push_back(palette);
    //        fragments.push_back(image);
    //    }
    //
    //    float skip = 2048.0 / (float)ITEMS;
    //    for (float x = 0; x < ofGetWindowWidth(); x += skip) {
    //        for (float y = 0; y < ofGetWindowHeight(); y += skip) {
    //            ofColor c = targetImage.getColor(x + skip / 2, y + skip / 2);
    //            int normX = x / skip;
    //            int normY = y / skip;
    //
    //            float closestMatch = 100000;
    //            int closestFragment = -1;
    //            for (int i = 0; i < fragmentColors.size(); i++) {
    //                for (int j = 0; j < fragmentColors[i].size(); j++) {
    //                    ofColor pc = fragmentColors[i][j];
    //                    float dist = ofDist(c.r, c.g, c.b, pc.r, pc.g, pc.b) +
    //                                 ofxGaussian() * 4;  // 28;
    //                    if (dist < closestMatch && fragmentCounters[i] < 12) {
    //                        closestMatch = dist;
    //                        closestFragment = i;
    //                    }
    //                }
    //            }
    //
    //            fragmentCounters[closestFragment < 0 ? 0 : closestFragment]++;
    //            mosaic[normX][normY] = closestFragment < 0 ? 0 :
    //            closestFragment; mosaicRotation[normX][normY] =
    //            c.getBrightness();
    //
    //            //            ofPath p;
    //            //            p.setColor(c);
    //            //            p.rectangle(x, y, skip, skip);
    //            //
    //            //
    //            //            paths.push_back(p);
    //        }
    //    }
}

float done = false;

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(ofColor::white);
    ofSetColor(ofColor::white);

    if (showOrg) {
        targetImage.draw(0, 0);
        return;
    }

    //    float w = colorImg.getWidth();
    //
    //    colorImg.draw(0, 0);
    //    grayImg.draw(w, 0);
    //
    //    for (int i = 0; i < contourFinder.nBlobs; i++) {
    //        contourFinder.blobs[i].draw(w * 2, 0);
    //
    //        ofTranslate(w * 3, 0);
    //
    //        ofSetColor(ofColor::black);
    //        ofDrawRectangle(0, 0, w, colorImg.getHeight());
    //
    //        ofFill();
    //        ofSetColor(ofColor::white);
    //        ofBeginShape();
    //        auto pts = contourFinder.blobs[i].pts;
    //        for (int j = 0; j < pts.size(); j++) {
    //            ofVertex(pts[j].x, pts[j].y);
    //        }
    //        ofEndShape(true);
    //    }

    //        if (showOrg) {

    //            return;
    //        }

    //    float skip = 2048.0 / (float)ITEMS;
    //    for (float x = 0; x < ofGetWindowWidth(); x += skip) {
    //        for (float y = 0; y < ofGetWindowHeight(); y += skip) {
    //            int normX = x / skip;
    //            int normY = y / skip;
    //            int fragmentIndex = mosaic[normX][normY];
    //            ofImage fragment = fragments[fragmentIndex];
    //
    //            ofPushMatrix();
    //            ofTranslate(x, y);
    //
    //            float iw = fragment.getWidth();
    //            float ih = fragment.getHeight();
    //
    //            ofColor c = targetImage.getColor(x, y);
    //
    //            float scale = iw < ih ? skip / ih : skip / iw;
    //            ofTranslate(skip / 2, skip / 2);
    //            ofScale(scale * ofMap(c.getBrightness(), 0, 255, 1.3, 1.0,
    //            true) *
    //                    1.5);
    //            ofPushMatrix();
    //            fragment.setAnchorPercent(0.5, 0.5);
    //            ofRotateDeg(mosaicRotation[normX][normY]);
    //            fragment.draw(0, 0);
    //            ofPopMatrix();
    //
    //            //            ofScale(0.3);
    //            //            fragment.draw(0, 0);
    //            ofPopMatrix();
    //        }
    //    }

    //
    //    for (int i = 0; i < paths.size(); i++) {
    //        paths[i].draw();
    //    }

    //    int imgIdx = ofMap(mouseX, 0, ofGetWidth(), 0, TOTAL_FRAGMENTS - 1,
    //    true);
    //
    //    fragments[imgIdx].draw(0, 100);
    //
    //    vector<ofColor> colors = fragmentColors[imgIdx];
    //    for (int i = 0; i < colors.size(); i++) {
    //        ofSetColor(colors[i]);
    //        ofDrawRectangle(500, i * 60 + 90, 60, 60);
    //    }
    //
    //    ofSetColor(ofColor::black);
    //    ofDrawBitmapString("i" + to_string(imgIdx) + ".jpg", 500, 50);
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

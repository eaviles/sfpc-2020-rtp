#include "ofApp.h"

bool hueCompare(ofColor a, ofColor b) { return a.getHue() < b.getHue(); }

//--------------------------------------------------------------
void ofApp::setup() {
    showOrg = false;
    targetImage.load("edgardo.jpg");

    for (int i = 0; i < totalFragments; i++) {
        cout << "Loading: " << i << endl;

        ofImage image;
        string file = "fragments/i" + to_string(i) + ".jpg";
        image.load(file);
        float imageWidth = image.getWidth();
        float imageHeight = image.getHeight();

        ofImage imageCopy;
        imageCopy.load(file);
        imageCopy.resize(imageWidth * 0.2, imageHeight * 0.2);
        ofxColorQuantizer colorQuantizer;
        colorQuantizer.setNumColors(4);
        colorQuantizer.quantize(imageCopy.getPixels());
        vector<ofColor> palette = colorQuantizer.getColors();
        ofSort(palette, hueCompare);

        float max = 0.0;
        int maxIndex = -1;
        for (int j = 0; j < palette.size(); j++) {
            if (palette[j].getBrightness() > max) {
                max = palette[j].getBrightness();
                maxIndex = j;
            }
        }
        palette.erase(palette.begin() + maxIndex);


        float avgR = 0;
        float avgG = 0;
        float avgB = 0;
        for (int j = 0; j < palette.size(); j++) {
            avgR += palette[j].r;
            avgG += palette[j].g;
            avgB += palette[j].b;
        }
        avgR /= palette.size();
        avgG /= palette.size();
        avgB /= palette.size();
        palette.clear();
        palette.push_back(ofColor(avgR, avgG, avgB));



        ofColor tl = image.getColor(10, 10);
        ofColor tr = image.getColor(imageWidth - 10, 10);
        ofColor bl = image.getColor(10, imageHeight - 10);
        ofColor br = image.getColor(imageWidth - 10, imageHeight - 10);
        ofColor tc = ofColor((tl.r + tr.r + bl.r + br.r) / 4.0,
                             (tl.g + tr.g + bl.g + br.g) / 4.0,
                             (tl.b + tr.b + bl.b + br.b) / 4.0);

        image.setImageType(OF_IMAGE_COLOR_ALPHA);
        for (int x = 0; x < imageWidth; x++) {
            for (int y = 0; y < imageHeight; y++) {
                ofColor c = image.getColor(x, y);
                if (ofDist(c.r, c.g, c.b, tc.r, tc.g, tc.b) < 32) {
                    image.setColor(x, y, ofColor(c.r, c.g, c.b, 0));
                } else {
                    image.setColor(x, y, ofColor(c.r, c.g, c.b, 255));
                }
            }
        }
        image.update();

        fragmentColors.push_back(palette);
        fragments.push_back(image);
    }

    float skip = 2048.0 / (float)ITEMS;
    for (float x = 0; x < ofGetWindowWidth(); x += skip) {
        for (float y = 0; y < ofGetWindowHeight(); y += skip) {
            ofColor c = targetImage.getColor(x + skip / 2, y + skip / 2);
            int normX = x / skip;
            int normY = y / skip;

            float closestMatch = 100000;
            int closestFragment = -1;
            for (int i = 0; i < fragmentColors.size(); i++) {
                for (int j = 0; j < fragmentColors[i].size(); j++) {
                    ofColor pc = fragmentColors[i][j];
                    float dist = ofDist(c.r, c.g, c.b, pc.r, pc.g, pc.b) + ofxGaussian() * 8;//28;
                    if (dist < closestMatch) {
                        closestMatch = dist;
                        closestFragment = i;
                    }
                }
            }

            mosaic[normX][normY] = closestFragment;
            mosaicRotation[normX][normY] = c.getBrightness();

            //            ofPath p;
            //            p.setColor(c);
            //            p.rectangle(x, y, skip, skip);
            //
            //
            //            paths.push_back(p);
        }
    }
}

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

    float skip = 2048.0 / (float)ITEMS;
    for (float x = 0; x < ofGetWindowWidth(); x += skip) {
        for (float y = 0; y < ofGetWindowHeight(); y += skip) {
            int normX = x / skip;
            int normY = y / skip;
            int fragmentIndex = mosaic[normX][normY];
            ofImage fragment = fragments[fragmentIndex];

            ofPushMatrix();
            ofTranslate(x, y);

            float iw = fragment.getWidth();
            float ih = fragment.getHeight();

            ofColor c = targetImage.getColor(x, y);

                float scale = iw < ih ? skip / ih : skip/iw;
                ofTranslate(skip / 2, skip / 2);
                ofScale(scale * ofMap(c.getBrightness(), 0, 255, 1.4, 1.2, true) * 1.5);
                ofPushMatrix();
                fragment.setAnchorPercent(0.5, 0.5);
                ofRotateDeg(mosaicRotation[normX][normY]);
                fragment.draw(0, 0);
                ofPopMatrix();

            //            ofScale(0.3);
            //            fragment.draw(0, 0);
            ofPopMatrix();
        }
    }

    //
    //    for (int i = 0; i < paths.size(); i++) {
    //        paths[i].draw();
    //    }

    //    int imgIdx = ofMap(mouseX, 0, ofGetWidth(), 0, totalFragments - 1,
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
void ofApp::mousePressed(int x, int y, int button) {
    showOrg = !showOrg;
}

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

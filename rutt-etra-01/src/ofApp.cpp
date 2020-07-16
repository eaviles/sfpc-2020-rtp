#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);

    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 60);
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_RGB8, 60);
    pipe.start(cfg);

    thrFilter.set_option(RS2_OPTION_MAX_DISTANCE, 1.0);
    tmpFilter.set_option(RS2_OPTION_FILTER_SMOOTH_ALPHA, 0.4);
    tmpFilter.set_option(RS2_OPTION_FILTER_SMOOTH_DELTA, 20.0);
    tmpFilter.set_option(RS2_OPTION_HOLES_FILL, 3);

    minX = -518.42;
    maxX = 513.25;
    minY = -314.85;
    maxY = 387.40;
    minZ = -1175.0;
    maxZ = -400;

    ofEnableDepthTest();
    ofSetupScreenPerspective();
    ofEnableSmoothing();

    cam.setAutoDistance(false);

    gui.setup("Settings", "settings.xml");
    gui.add(fps.set("FPS", "?"));
    gui.add(
        cameraOrientation.set("orientation", vec3(0), vec3(-180), vec3(180)));
    gui.add(cameraPosition.set("position", vec3(0, 0, 1200), vec3(-2400),
                               vec3(2400)));
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
    cam.setOrientation(cameraOrientation);
    cam.setPosition(cameraPosition);

    if (!pipe.poll_for_frames(&frames)) return;
    rs2::align align_to_color(RS2_STREAM_COLOR);
    frames = align_to_color.process(frames);

    auto depth = frames.get_depth_frame();
    int width = depth.get_width();
    int height = depth.get_height();

    depth = thrFilter.process(depth);
    depth = tmpFilter.process(depth);

    auto color = frames.get_color_frame();
    pc.map_to(color);
    points = pc.calculate(depth);
    int numOfPoints = points.size();
    auto colors = (const unsigned char*)color.get_data();

    meshes.clear();

    if (numOfPoints != offsets.size()) {
        offsets.clear();
        for (int i = 0; i < numOfPoints; i++) {
            offsets.push_back(
                {ofRandom(0, 10000), ofRandom(0, 10000), ofRandom(0, 10000)});
        }
    }

    if (numOfPoints == 0) return;
    float t = ofGetElapsedTimef();

    auto vertices = points.get_vertices();
    for (int y = 0; y < height; y += 5) {
        ofMesh newMesh;
        meshes.push_back(newMesh);
        meshes.back().setMode(OF_PRIMITIVE_LINE_STRIP);
        meshes.back().enableColors();

        for (int x = 0; x < width; x += 10) {
            int i = x + y * width;

            if (vertices[i].z) {
                auto v = vertices[i];
                float vx = v.x * 1000;
                float vy = v.y * 1000;
                float vz = (v.z - 1.4) * 1000;

                if (vx < minX) minX = vx;
                if (vx > maxX) maxX = vx;
                if (vy < minY) minY = vy;
                if (vy > maxY) maxY = vy;
                if (vz < minZ) minZ = vz;
                if (vz > maxZ) maxZ = vz;

                auto offset = offsets[i];

                if (y > (float)height * .3 && y < (float)height * .6) {
                    float displacement = 320.0 * ((sin(t * .5) + 1.0) / 2.0);
                    vx += ofNoise(offset.x + t * 6.0) * displacement / 10.0;
                    vy += ofNoise(offset.y + t * 6.0) * displacement / 10.0;
                    float vzd = ofNoise(offset.z + t * 6.0) * displacement;
                    vzd *= ofNoise((float)x * .0075, (float)y * .0075, t * .5);
                    vzd *= ofMap(y, 0, height - 1, 1.0, 0, true);
                    vz -= vzd;
                }

                meshes.back().addVertex(vec3(vx, vy, vz));
                float r = colors[i * 3];
                float g = colors[(i * 3) + 1];
                float b = colors[(i * 3) + 2];
                meshes.back().addColor(ofColor(r, g, b));
            } else {
                float vx = ofMap(x, 0, width - 1, minX, maxX, true);
                float vy = ofMap(y, 0, height - 1, minY, maxY, true);
                float vz = maxZ;

                auto offset = offsets[i];

                float displacement = 160.0 + 80.0 * ((sin(t * .5) + 1.0) / 2.0);
                vx += ofNoise(offset.x + t * 3.0) * displacement / 5.0;
                vy += ofNoise(offset.x + t * 3.0) * displacement / 5.0;
                float vzd = ofNoise(offset.z + t * 2.0) * displacement;
                vzd *= ofNoise((float)x * .0075, (float)y * .0075, t * .5);
                vz -= vzd;

                float cdy = ofMap(y, 0, height - 1, 1.0, 0);
                float cdz = ofMap(vzd * 1.0, 0, displacement, .25, 1, true);
                float cd = cdy * cdz;

                meshes.back().addVertex(vec3(vx, vy, vz));
                meshes.back().addColor(ofColor(cd * 128));
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    ofEnableDepthTest();

    ofPushMatrix();
    cam.begin();
    ofScale(1, -1, -1);
    ofSetLineWidth(6.0);
    for (int i = 0; i < meshes.size(); i++) {
        meshes[i].draw();
    }
    cam.end();
    ofPopMatrix();
    ofDisableDepthTest();

    if (SHOW_GUI) {
        fps = ofToString(ofGetFrameRate(), 2);
        gui.draw();
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
    if (SHOW_GUI) {
        cameraOrientation = cam.getOrientationEulerDeg();
        cameraPosition = cam.getPosition();
    }
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

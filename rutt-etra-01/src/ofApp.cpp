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

    glm::vec3 vals = {0.0, 0.0, 0.0};
    glm::vec3 mins = {-2400, -2400, -2400};
    glm::vec3 maxs = {2400, 2400, 2400};
    gui.setup("Settings", "settings.xml");
    gui.add(fps.set("FPS", "?"));
    gui.add(cameraTarget.set("target", vals, mins, maxs));
    gui.add(cameraOrientation.set("orientation", vals, mins, maxs));
    gui.add(cameraPosition.set("position", {0, 0, 1200}, mins, maxs));
    gui.loadFromFile("settings.xml");
}

//--------------------------------------------------------------
void ofApp::update() {
    if (pipe.poll_for_frames(&frames)) {
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
        auto colors = (const unsigned char*)color.get_data();

        meshes.clear();

        if (points.size() > 0) {
            auto vertices = points.get_vertices();
            for (int y = 0; y < height; y += 5) {
                ofMesh newMesh;
                meshes.push_back(newMesh);
                meshes.back().setMode(OF_PRIMITIVE_LINE_STRIP);
                meshes.back().enableColors();

                for (int x = 0; x < width; x += 5) {
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

                        meshes.back().addVertex(glm::vec3(vx, vy, vz));
                        float r = colors[i * 3];
                        float g = colors[(i * 3) + 1];
                        float b = colors[(i * 3) + 2];
                        meshes.back().addColor(ofColor(r, g, b));
                    } else {
                        float vx = ofMap(x, 0, width - 1, minX, maxX, true);
                        float vy = ofMap(y, 0, height - 1, minY, maxY, true);
                        float vz = maxZ;
                        meshes.back().addVertex(glm::vec3(vx, vy, vz));
                        meshes.back().addColor(
                            ofColor(ofMap(y, 0, height - 1, 255 * 0.25, 0)));
                    }
                }
            }
        }
    }

    cam.setTarget(cameraTarget);
    cam.setOrientation(cameraOrientation);
    cam.setPosition(cameraPosition);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);
    ofEnableDepthTest();

    ofPushMatrix();
    cam.begin();
    ofScale(1, -1, -1);
    ofSetLineWidth(5.0);
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
    cameraTarget = cam.getTarget().getOrientationEulerDeg();
    cameraOrientation = cam.getOrientationEulerDeg();
    cameraPosition = cam.getPosition();
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

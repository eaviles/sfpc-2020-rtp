#pragma once

#include "ofMain.h"

typedef array<int, 16> magicSquare;

class ofApp : public ofBaseApp{
    
    public:
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y );
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        magicSquare durerSquare = {
            16,  3,  2, 13,
             5, 10, 11,  8,
             9,  6,  7, 12,
             4, 15, 14,  1
        };
        magicSquare randomSquares[25];
    
};

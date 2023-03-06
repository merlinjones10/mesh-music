#pragma once

#include "ofMain.h"
#include "NoteBlob.hpp"
#include "ofxOsc.h"

class ofApp : public ofBaseApp {
    public:
        void setup();
        void update();
        void draw();
        
        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void gotMessage(ofMessage msg);
        
        ofEasyCam cam;
        ofMesh mesh;
        ofImage img;
    bool learn;
    
        float liquidness;
        float speedDampen;
    
        vector<NoteBlob> noteBlobs;
    
        ofxOscSender oscSender;
    
    void onBangInAnyBlob(glm::vec3 & e);
    float startTime;
    float timerInterval;
    float beatTicker; 
    
    ofVideoGrabber vidGrabber;
    ofPixels videoInverted;
    ofTexture videoTexture;
    int camWidth;
    int camHeight;



};


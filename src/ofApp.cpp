#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    img.load("bach.png");
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    int skip = 2;
    for(int y = 0; y < img.getHeight(); y += skip) {
        for(int x = 0; x < img.getWidth() -1; x += skip) {
            ofColor cur = img.getColor(x, y);
            if(cur.getBrightness() < 10) {
                glm::vec3 pos(x, y, 0);
                NoteBlob newNoteBlob(pos);
                noteBlobs.push_back(newNoteBlob);
            }
            else {
                float z = 0.0;
                cur.a == 255;
                mesh.addColor(ofColor(ofRandom(100, 110), ofRandom(50, 80), ofRandom(200, 255)));
                glm::vec3 pos(x, y, z);
                mesh.addVertex(pos);
            }
        }
    }
    
    ofEnableDepthTest();
    glEnable(GL_POINT_SMOOTH);
    glPointSize(4); // make the points bigger
}

//--------------------------------------------------------------

void ofApp::update() {
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update();
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);
    cam.begin();
    ofScale(2, -2, 2); // flip the y axis and zoom in a bit
    //    ofRotateYDeg(90);
    ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
    mesh.draw();
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].draw();
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            for (int i = 0; i<noteBlobs.size(); i++) {
                noteBlobs[i].reset();
            }
            break;
            
        default:
            break;
    }
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}


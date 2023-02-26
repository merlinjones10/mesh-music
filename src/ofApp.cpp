#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(60);
    ofSetSphereResolution(3);
    ofSetVerticalSync(true);
    img.load("bach.png");
    img.resize(img.getWidth() / 1, img.getHeight() / 1);
    
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
                float z = 1.0;
                cur.a == 255;
                mesh.addColor(ofColor(0, 0, 0));
                glm::vec3 pos(x, y, z);
                mesh.addVertex(pos);
            }
        }
    }
    
    ofEnableDepthTest();
//    glPointSize(2);
    
    liquidness = 1.0;
    speedDampen = 20.0;
    
}

//--------------------------------------------------------------

void ofApp::update() {
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update(liquidness, 1.0);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0,0,0);
    cam.begin();
    ofScale(2, -2, 2);
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
            
        case OF_KEY_LEFT:
            if (liquidness > 0) {
                liquidness -= 0.5;
            }
            std::cout << "Lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_RIGHT:
            liquidness += 0.5;
            std::cout << "Lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_DOWN:
            if (speedDampen> 0) {
                speedDampen -= 0.5;
            }
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case OF_KEY_UP:
            speedDampen += 0.5;
            std::cout << "Speed: " << speedDampen << std::endl;
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


#include "ofApp.h"
#define HOST "localhost"
#define PORT 12346
#define SCALE 3
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
//    ofSetSphereResolution(10);
//    glPointSize(2);
    ofSetVerticalSync(true);
    img.load("bach.png");
    img.resize(img.getWidth() / 1, img.getHeight() / 1);
    oscSender.setup(HOST, PORT);
    mesh.setMode(OF_PRIMITIVE_POINTS);

    int skip = 2;
    for(int y = 0; y < img.getHeight(); y += skip) {
        for(int x = 0; x < img.getWidth() -1; x += skip) {
            ofColor cur = img.getColor(x, y);
            if(cur.getBrightness() < 10) {
                glm::vec3 pos(x * SCALE, y * SCALE, 3);
                NoteBlob newNoteBlob(pos);
                noteBlobs.push_back(newNoteBlob);
            }
            else {
                float z = 1.0;
                cur.a == 255;
                mesh.addColor(ofColor(10, 10, 10));
                glm::vec3 pos(x * SCALE, y * SCALE, z * SCALE);
                mesh.addVertex(pos);
            }
        }
    }

    ofAddListener(NoteBlob::onBlobBangGlobal , this, &ofApp::onBangInAnyBlob);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    liquidness = 1.0;
    speedDampen = 0.01;
    ofBackground(0,0,0);
    ofxOscMessage m;

    
    int highestX = noteBlobs[0].position.x;
    int lowestX = noteBlobs[0].position.x;
    int highestY = noteBlobs[0].position.y;
    int lowestY = noteBlobs[0].position.y;
    
    
    for (NoteBlob noteBlob : noteBlobs) {
        if (noteBlob.position.x > highestX) {
            highestX = noteBlob.position.x;
        }
        if (noteBlob.position.y > highestY) {
            highestY = noteBlob.position.y;
        }
        if (noteBlob.position.x < lowestX) {
             lowestX = noteBlob.position.x;
        }
        if (noteBlob.position.y < lowestY) {
             lowestY = noteBlob.position.y;
        }
    }
    m.setAddress("/minmax/x");
    m.addIntArg(lowestX);
    m.addIntArg(highestX);
    oscSender.sendMessage(m, false);
    
    ofxOscMessage m2;
    m2.setAddress("/minmax/y");
    m2.addIntArg(lowestY);
    m2.addIntArg(highestY);
    ofLog() << lowestX << " || " << highestX;
    ofLog() << lowestY << " || " << highestY;

    oscSender.sendMessage(m2, false);


}

//--------------------------------------------------------------
void ofApp::onBangInAnyBlob(glm::vec3 & e){
        ofxOscMessage m;
        m.setAddress("/blip/position");
        m.addIntArg(e.x);
        m.addIntArg(e.y);
        oscSender.sendMessage(m, false);
}

void ofApp::update() {
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update(speedDampen);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    ofScale(2, -2, 2);
    ofTranslate(-img.getWidth() * SCALE / 2, -img.getHeight() * SCALE / 2 );
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
                speedDampen -= 0.001;
            }
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case OF_KEY_UP:
            speedDampen += 0.001;
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


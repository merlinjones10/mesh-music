#include "ofApp.h"
#define HOST "localhost"
#define PORT 12346
#define SCALE 3

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetSphereResolution(3);
    oscSender.setup(HOST, PORT);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    camWidth = 640;
    camHeight = 480;
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
        }
    }
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.setup(camWidth, camHeight);
    img.setFromPixels(vidGrabber.getPixels());
    learn = false;
    ofAddListener(NoteBlob::onBlobBangGlobal , this, &ofApp::onBangInAnyBlob);
    ofEnableDepthTest();
    ofEnableAlphaBlending();
    liquidness = -320.0;
    speedDampen = -260.0;
    ofBackground(0,0,0);
    ofxOscMessage m;
    seed = ofRandom(1, 1000);
    ofLog() << seed;

}
//--------------------------------------------------------------

void ofApp::takePhoto() {
    float threshold = 90;
    mesh.clear();
    noteBlobs.clear();
    ofPixels & pixels = vidGrabber.getPixels();
    pixels.resize(pixels.getWidth() , pixels.getHeight());
    ofPixels & hiDefPixels = vidGrabber.getPixels();
    hiDefPixels.resize(hiDefPixels.getWidth() , hiDefPixels.getHeight());
    int skip = 2;
    ofColor tempCol;
        for(int y = 0; y < pixels.getHeight(); y += skip) {
            for(int x = 0; x < pixels.getWidth(); x += skip) {
                ofColor cur = pixels.getColor(x, y);
                if(cur.getBrightness() < threshold) {
                    glm::vec3 pos(x, -y, 10);
                    NoteBlob newNoteBlob(pos);
                    noteBlobs.push_back(newNoteBlob);
                }
            }
        }
    for(int y = 0; y < hiDefPixels.getHeight(); y++) {
        for(int x = 0; x < hiDefPixels.getWidth(); x ++) {
            ofColor cur = pixels.getColor(x, y);
            if(cur.getBrightness() < threshold) {
                tempCol.setHsb(200, 100, 20, 200);
                tempCol.a = 1.0;
                hiDefPixels.setColor(x, y, tempCol);
            }
            else {
                tempCol.setHsb(0, 0, 0, 200);
                tempCol.a = 1.0;
                hiDefPixels.setColor(x, y, tempCol);
            }
        }
    }
        img.setFromPixels(hiDefPixels);
        ofxOscMessage m;
        m.setAddress("/minmax/y");
        m.addIntArg(noteBlobs[0].position.y);
        m.addIntArg(noteBlobs.back().position.y);
        oscSender.sendMessage(m, false);
    
    
    
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
    vidGrabber.update();
    if (learn) {
        takePhoto();
        learn = false;
    }
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update(seed);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    
    ofSetColor(255);
    vidGrabber.draw(-ofGetWidth() / 2 , (ofGetHeight() / 2) - vidGrabber.getHeight() / 4, vidGrabber.getWidth() / 4, vidGrabber.getHeight() /4 );
    img.draw(liquidness, speedDampen);

    ofTranslate(-310,230, 180);
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].draw();
    }
//    mesh.draw();
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
          
                liquidness -= 2;
            
            std::cout << "Lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_RIGHT:
            liquidness += 2;
            std::cout << "Lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_DOWN:
         
                speedDampen -= 2;
            
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case OF_KEY_UP:
            speedDampen += 2;
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case 49:
            learn = true;
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


#include "ofApp.h"
#define HOST "localhost"
#define PORT 12346
#define SCALE 1
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetSphereResolution(6);
    //    glPointSize(2);
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
    vidGrabber.setVerbose(true);
    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.setup(camWidth, camHeight);
    
    videoInverted.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    img.setFromPixels(vidGrabber.getPixels());
    
    
    learn = false;
    ofAddListener(NoteBlob::onBlobBangGlobal , this, &ofApp::onBangInAnyBlob);
    //    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    liquidness = 1.0;
    speedDampen = 70.0;
        ofBackground(255);
    startTime = ofGetElapsedTimeMillis();
    timerInterval = 125;
    beatTicker = 0.0;
    std::cout << ofGetElapsedTimeMillis() << '\n';
    mode = 0;
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
        ofLog() << "Learn";

        takePhoto();
        learn = false;
    }
    
    float timer = ofGetElapsedTimeMillis() - startTime;
    float count = (int)beatTicker % (int)img.getWidth();
    if (timer >= timerInterval) {
        //        elapsedBeat = true;
        if (mode == 0) { // rando ando
            if (noteBlobs.size() > 0) {
                if (ofRandom(100) < 10) {
                    noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
                }
                if (ofRandom(100) < 2) {
                    noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
                }
                if (ofRandom(100) < 5) {
                    noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
                }
            }
        }
        if (mode == 1) { // L R seq
            for (int i = 0; i<noteBlobs.size(); i++) {
                if (noteBlobs[i].position.x  == count) {
                    noteBlobs[i].setBang(true);
                }
            }
        }
        beatTicker++;
        startTime = ofGetElapsedTimeMillis();
    }
    
    //
    float liquidness = 285;
    float amplitude = 1.1;
    float speedDampen = 5;
    if (noteBlobs.size() > 0 ) {
        for(unsigned int i = 0; i < noteBlobs.size(); i++){
    //        noteBlobs[i].position.x += ofSignedNoise(noteBlobs[i].position.x/liquidness, noteBlobs[i].position.y/liquidness,noteBlobs[i].position.z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
    //        noteBlobs[i].position.y += ofSignedNoise(noteBlobs[i].position.z/liquidness, noteBlobs[i].position.x/liquidness,noteBlobs[i].position.y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            noteBlobs[i].position.z += ofSignedNoise(noteBlobs[i].position.y/liquidness, noteBlobs[i].position.z/liquidness,noteBlobs[i].position.x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            noteBlobs[i].update(count);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
//    ofTranslate(-img.getWidth() / 2, img.getHeight()/ 2);
    
    ofSetColor(255);
    img.draw(0, -img.getHeight());
    
//        mesh.draw();
//    vidGrabber.draw(0, ofGetHeight());
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].draw();
    }
    cam.end();
}

void ofApp::takePhoto() {
    float threshold = speedDampen;
    mesh.clear();
    noteBlobs.clear();
    if(vidGrabber.isFrameNew()){
        ofPixels & pixels = vidGrabber.getPixels();
        pixels.resize(pixels.getWidth() / 2, pixels.getHeight() / 2);
        
        int skip = 1;
        for(int y = 0; y < pixels.getHeight(); y += skip) {
            for(int x = 0; x < pixels.getWidth(); x += skip) {
                ofColor cur = pixels.getColor(x, y);
                if(cur.getBrightness() < threshold) {
                    // 120
                    glm::vec3 pos(x, -y, 5);
                    NoteBlob newNoteBlob(pos);
                    noteBlobs.push_back(newNoteBlob);
                    x += 4;
                }
                else {
                    ofColor tempCol;
                    tempCol.setHsb(0, 0, 255);
                    tempCol.a = 1.0;
                    pixels.setColor(x, y, tempCol);
                    //                        float z = 0.0;
                    //                        cur.a == 255;
                    //                        mesh.addColor(ofColor(10, 10, 10));
                    //                        glm::vec3 pos(x , -y , z );
                    //                        mesh.addVertex(pos);
                }
            }
        }
        img.setFromPixels(pixels);
        ofxOscMessage m;
        m.setAddress("/minmax/y");
        m.addIntArg(noteBlobs[0].position.y);
        m.addIntArg(noteBlobs.back().position.y);
        oscSender.sendMessage(m, false);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    std::cout << key;
    switch (key) {
        case 49: { // 1 key 
            ofLog() << "Im learning";
            learn = true;
            break;
        }
        case ' ':
            for (int i = 0; i<noteBlobs.size(); i++) {
                noteBlobs[i].reset();
            }
            break;
            
        case OF_KEY_LEFT:
            if (mode > 0) {
                mode -= 1;
            }
            std::cout << "mode: " << liquidness << std::endl;
            break;
        case OF_KEY_RIGHT:
            mode += 1;
            std::cout << "mode: " << liquidness << std::endl;
            break;
        case OF_KEY_DOWN:
            if (speedDampen> 0) {
                speedDampen -= 1.0;
            }
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case OF_KEY_UP:
            speedDampen += 1.0;
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


#include "ofApp.h"
#define HOST "localhost"
#define PORT 12346
#define SCALE 1
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetSphereResolution(6);
//    glPointSize(2);
//    img.load("bach.png");
//    img.resize(img.getWidth() / 1, img.getHeight() / 1);
    oscSender.setup(HOST, PORT);
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    camWidth = 320 ;  // try to grab at this size.
    camHeight = 240;
    vector<ofVideoDevice> devices = vidGrabber.listDevices();
    for(size_t i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){
            //log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
        }else{
            //log the device and note it as unavailable
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

    
    learn = true;
    ofAddListener(NoteBlob::onBlobBangGlobal , this, &ofApp::onBangInAnyBlob);
//    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    liquidness = 1.0;
    speedDampen = 1.0;
//    ofBackground(0,0,0);
    startTime = ofGetElapsedTimeMillis();
    timerInterval = 125;
    beatTicker = 0.0;
    std::cout << ofGetElapsedTimeMillis() << '\n';
}

//--------------------------------------------------------------
void ofApp::onBangInAnyBlob(glm::vec3 & e){
        ofxOscMessage m;
        m.setAddress("/blip/position");
        m.addIntArg(e.x);
        m.addIntArg(e.y);
        oscSender.sendMessage(m, false);
}
bool rollDice(float probability){
    return ofRandom(100) > probability;
}
void ofApp::update() {

    vidGrabber.update();

    if (learn) {
        float threshold = 90.0;
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
                        glm::vec3 pos(x, -y, 22);
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
        }
        learn = false;
    }

    //
    float timer = ofGetElapsedTimeMillis() - startTime;
    float count = (int)beatTicker % (int)img.getWidth();

    
    if (timer >= timerInterval) {
        if (noteBlobs.size() > 0) {
            if (ofRandom(100) < 50) {
                noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
            }
            if (ofRandom(100) < 25) {
                noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
            }
            if (ofRandom(100) < 10) {
                noteBlobs[ofRandom(noteBlobs.size() - 1)].setBang(true);
            }
        }
//        for (int i = 0; i<noteBlobs.size(); i++) {
//            noteBlobs[i].setBang(true);
//        }
        beatTicker++;
        startTime = ofGetElapsedTimeMillis();
    }
    
    // regular update
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update(count);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    ofTranslate(-img.getWidth() / 2, img.getHeight()/ 2);

    ofSetColor(255);
    img.draw(0,-img.getHeight());

//    mesh.draw();
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].draw();
    }
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
            
        case 'l': {
            learn = true;
            break;
        }
        case ' ':
            for (int i = 0; i<noteBlobs.size(); i++) {
                noteBlobs[i].reset();
            }
            break;
            
        case OF_KEY_LEFT:
//            if (liquidness > 0) {
                liquidness -= 1.0;
//            }
            std::cout << "Lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_RIGHT:
            liquidness += 1.0;
            std::cout << "Lquid: " << liquidness << std::endl;
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


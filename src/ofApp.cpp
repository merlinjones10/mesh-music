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
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    liquidness = 100.0;
    speedDampen = 5.0;
    ofBackground(10);

    startTime = ofGetElapsedTimeMillis();
    timerInterval = 1000;
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
        liquidness = ofRandom(100) + 1;
        speedDampen = ofRandom(20) + 1;
        beatTicker = 0;
    }
    
    float timer = ofGetElapsedTimeMillis() - startTime;
    float count = (int)beatTicker % (int)img.getWidth();
    if (timer >= timerInterval) {
        beatTicker++;
        ofLog() << "Beat: " << beatTicker;
        startTime = ofGetElapsedTimeMillis();
    }
    float amplitude = 0.1;
    if (noteBlobs.size() > 0 ) {
        for(unsigned int i = 0; i < noteBlobs.size(); i++){
            float noiseVal = ofSignedNoise(noteBlobs[i].position.y/liquidness, noteBlobs[i].position.x/liquidness,noteBlobs[i].position.x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            if (beatTicker > 20) {
                noiseVal = -abs(noiseVal); // after 20 secs head home...
            }
            noteBlobs[i].update(noiseVal);
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
    float threshold = 70;
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

                    int randomStart = ofRandom(5, 15);
                    glm::vec3 pos(x, -y, randomStart); // testing out ways to stop the mass notes at the start of a picture, this range could be extended OR shortened for effect? so every picture behaves differently
                    // the noise values are also slightly different so there seems to be enough variation....
                    // last thing might be to receive midi to take the photo on command from pad rather than keyboard.
                    NoteBlob newNoteBlob(pos);
                    noteBlobs.push_back(newNoteBlob);
                    x += 4;
                }
                else {
                    ofColor tempCol;
                    tempCol.setHsb(0, 0, 0);
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
                liquidness -= 10;
            std::cout << "lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_RIGHT:
            liquidness += 10;
            std::cout << "lquid: " << liquidness << std::endl;
            break;
        case OF_KEY_DOWN:
    
                speedDampen -= 5.0;
            
            std::cout << "Speed: " << speedDampen << std::endl;
            break;
        case OF_KEY_UP:
            speedDampen += 5.0;
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


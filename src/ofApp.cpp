#include "ofApp.h"
#define HOST "localhost"
#define PORT 12346
#define SCALE 1
//--------------------------------------------------------------
void ofApp::setup() {
    ofSetFrameRate(30);
    ofSetSphereResolution(6);
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
    ofBackground(101);
    startTime = ofGetElapsedTimeMillis();
    timerInterval = 1000;
    beatTicker = 0.0;
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
        liquidness = ofRandom(800) + 1;
        ofLog() << "Lquid: " << liquidness << " | " << "Speed D: " <<  speedDampen;
        beatTicker = 0;
    }
    float timer = ofGetElapsedTimeMillis() - startTime;
    float count = (int)beatTicker % (int)img.getWidth();
    if (timer >= timerInterval) {
        beatTicker++;
        startTime = ofGetElapsedTimeMillis();
    }
    float amplitude = 0.1;
    if (noteBlobs.size() > 0 ) {
        for(unsigned int i = 0; i < noteBlobs.size(); i++){
            float noiseVal = ofSignedNoise(noteBlobs[i].position.y/liquidness, noteBlobs[i].position.x/liquidness,noteBlobs[i].position.x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            if (beatTicker > 100) {
                noiseVal = -abs(noiseVal); // after 20 secs head home...
            }
            noteBlobs[i].update(noiseVal);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    
    ofSetColor(255);
    
//    img.draw(0, -img.getHeight());
    mesh.draw(); // change to image ?? doesnt need to be a mesh ...
   
    vidGrabber.draw(-700, -img.getHeight() + 400);
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].draw();
    }
    cam.end();
}

void ofApp::takePhoto() {
    float threshold = 90;
    mesh.clear();
    noteBlobs.clear();
        ofPixels & pixels = vidGrabber.getPixels();
        pixels.resize(pixels.getWidth() , pixels.getHeight());
        
        int skip = 3;
        for(int y = 0; y < pixels.getHeight(); y += skip) {
            for(int x = 0; x < pixels.getWidth(); x += skip) {
                ofColor cur = pixels.getColor(x, y);
                if(cur.getBrightness() < threshold) {
                    glm::vec3 pos(x, -y, 10);
                    NoteBlob newNoteBlob(pos);
                    noteBlobs.push_back(newNoteBlob);
                    x += skip;
                }
                else {
                    ofColor tempCol;
                    tempCol.setHsb(0, 0, 0);
                    tempCol.a = 1.0;
                    pixels.setColor(x, y, tempCol);
                    float z = 0.0;
                    cur.a == 255;
                    mesh.addColor(ofColor(10, 10, 10));
                    glm::vec3 pos(x , -y , z );
                    mesh.addVertex(pos);
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


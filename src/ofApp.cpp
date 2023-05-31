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
    ofBackground(10);
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
        takePhoto();
        learn = false;
//        liquidness = ofRandom(800) + 1;
        int liquidChoices[3] = {800, 100, 400};
        liquidness = liquidChoices[mode];
        ofLog() << "Lquid: " << liquidness << " | " << "Speed D: " <<  speedDampen;
        beatTicker = 0;
    }
    float timer = ofGetElapsedTimeMillis() - startTime;
    float count = (int)beatTicker % (int)img.getWidth();
    if (timer >= timerInterval) {
        beatTicker++;
        startTime = ofGetElapsedTimeMillis();
    }
    float amplitude = 0.0;
    if (ofRandom(1000) > 800) {
        amplitude = 0.5;
    }
    if (noteBlobs.size() > 0 ) {
        for(unsigned int i = 0; i < noteBlobs.size(); i++){
            float noiseVal = ofSignedNoise(noteBlobs[i].position.y/liquidness, noteBlobs[i].position.x/liquidness,noteBlobs[i].position.x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
            if (beatTicker > 50) {
                noiseVal = -abs(noiseVal);
            }
            noteBlobs[i].update(noiseVal);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    
    ofSetColor(255);
    vidGrabber.draw(-ofGetWidth() / 2 , (ofGetHeight() / 2) - vidGrabber.getHeight() / 4, vidGrabber.getWidth() / 4, vidGrabber.getHeight() /4 );
    ofTranslate(-310,230, 180);
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
    int skipPresets[3] = {4, 2, 8};
    int skip = skipPresets[mode];
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
    ofLog() << key;
    switch (key) {
        case 49: { // 1 key
            mode = 0;
            learn = true;
            break;
        }
        case 50: { // 1 key
            mode = 1;
            learn = true;
            break;
        }
        case 51: { // 1 key
            mode = 2;
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


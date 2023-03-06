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
    vidGrabber.initGrabber(camWidth, camHeight);

    videoInverted.allocate(vidGrabber.getWidth(), vidGrabber.getHeight(), OF_PIXELS_RGB);
    videoTexture.allocate(videoInverted);
    img.setFromPixels(vidGrabber.getPixels());

    
    learn = true;
//    int skip = 1;
//    for(int y = 0; y < img.getHeight(); y += skip) {
//        for(int x = 0; x < img.getWidth() -1; x += skip) {
//            ofColor cur = img.getColor(x, y);
//            if(cur.getBrightness() < 10) {
//                glm::vec3 pos(x * SCALE, y * SCALE, 22);
//                NoteBlob newNoteBlob(pos);
//                noteBlobs.push_back(newNoteBlob);
//            }
//            else {
//                float z = 1.0;
//                cur.a == 255;
//                mesh.addColor(ofColor(10, 10, 10));
//                glm::vec3 pos(x * SCALE, y * SCALE, z * SCALE);
//                mesh.addVertex(pos);
//            }
//        }
//    }
    

    ofAddListener(NoteBlob::onBlobBangGlobal , this, &ofApp::onBangInAnyBlob);
//    ofEnableDepthTest();
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    liquidness = 1.0;
    speedDampen = 1.0;
//    ofBackground(0,0,0);

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
        mesh.clear();
        noteBlobs.clear();
        if(vidGrabber.isFrameNew()){
            ofPixels & pixels = vidGrabber.getPixels();
            
            for(size_t i = 0; i < pixels.size(); i++){
                //invert the color of the pixel
                videoInverted[i] = 255 - pixels[i];
            }
            //load the inverted pixels
            videoTexture.loadData(videoInverted);
            int skip = 1;

            
            
            for(int y = 0; y < pixels.getHeight(); y += skip) {
                for(int x = 0; x < pixels.getWidth(); x += skip) {
                    ofColor cur = pixels.getColor(x, y);
                    if(cur.getBrightness() < 60) {
                        // 120
                        glm::vec3 pos(x, -y, 22);
                        NoteBlob newNoteBlob(pos);
                        noteBlobs.push_back(newNoteBlob);
                        x += 4;
//                        y += 4;
                        
                    }
                    else {
                        ofColor tempCol;
                        tempCol.setHsb(0, 0, 0, 255);
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
    for (int i = 0; i<noteBlobs.size(); i++) {
        noteBlobs[i].update(speedDampen);
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    cam.begin();
    ofTranslate(-img.getWidth() / 2, img.getHeight()/ 2);

    ofSetColor(255);
    img.draw(0,-img.getHeight());
//    videoTexture.draw(0 , -img.getHeight());

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


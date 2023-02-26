#include "ofMain.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12346

NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
    oscSender.setup(HOST, PORT);
}


void NoteBlob::update(float liquidness, float speedDampen){
    position.z += ofSignedNoise( position.x, ofGetElapsedTimef() ,position.x) * liquidness;

    
    if (abs(position.z) < 0.2) {
        blip.size = 2;
        sendMesg();
//        position.z = position.z + position.z;
//        if (position.z > 0) {
//            position.z = 3;
//        } else {
//            position.z = -3;
//        }
    }
    blip.update();
}

void NoteBlob::draw(){
    ofSetColor(color);
    blip.draw();
    ofNoFill();
//    ofSetSphereResolution(3);
//    ofDrawSphere(position, size);
    ofFill();
}

void NoteBlob::reset(){
    position.z = 0; 
}

void NoteBlob::sendMesg(){
    ofxOscMessage m;
    m.setAddress("/blip/position");
    
    m.addIntArg(position.x);
    m.addIntArg(position.y);
    oscSender.sendMessage(m, false);

    
}

#include "ofMain.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"


NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
}


void NoteBlob::update(float liquidness, float speedDampen){
    position.z += ofSignedNoise(position.x / liquidness, position.y, ofGetElapsedTimef()/ speedDampen) * 0.2;
    
    float absoluteVal = abs(position.z);
    bool applyForce = false;
    
    if (absoluteVal < 0.2) {
        blip.size = 2;
        if (position.z < 0) {
            position.z = 0.3;
        } else {
            position.z = -0.3;
        }
        applyForce = true;
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


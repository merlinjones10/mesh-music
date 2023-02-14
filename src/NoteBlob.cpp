#include "ofMain.h"
#include "NoteBlob.hpp"

NoteBlob::NoteBlob(glm::vec3 pos) {
    position = pos;
    size = 1;
    color.set(ofRandom(100),ofRandom(100),ofRandom(2));
}


void NoteBlob::update(){
//    position.z = position.z + ofMap(ofNoise(ofRandom(10), (ofGetElapsedTimef() * 0.5)), 0, 1, -2, 2);

    float liquidness = 50;
    float amplitude = 2.0;
    float speedDampen = 20;

    position.z += ofSignedNoise(position.z/liquidness, position.y/liquidness,position.x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
}

void NoteBlob::draw(){
    ofSetColor(color);
    ofDrawSphere(position, size);
}

void NoteBlob::reset(){
    position.z = 0; 
}


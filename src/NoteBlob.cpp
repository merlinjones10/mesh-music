#include "ofMain.h"
#include "NoteBlob.hpp"

NoteBlob::NoteBlob(glm::vec3 pos) {
    position = pos;
    size = 1;
    color.set(ofRandom(100),ofRandom(100),ofRandom(2));
}


void NoteBlob::update(){
    //        verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
    //        verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
    //        verts[i].z += ofNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;//

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


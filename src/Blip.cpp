#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    position = pos;
    size = 0;
//    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
    color.setHsb(ofMap(position.y, 54, 690, 100, 120), 255, 255);
    fallSpeed = ofRandom(0.5, 1.0);
}

void Blip::update(glm::vec3 parentPos){
    if (size >= 5) {
        position = parentPos;
        color.setHsb(ofMap(position.y, 54, 690, 100, 120), 255, 255);
    }
    if (size > 0.001 ) {
        size -=0.05;
        position.z -= fallSpeed;
        color.setBrightness(color.getBrightness() - 2.5);
        float amount3d = ofSignedNoise(position.x / 10, position.y / 10, ofGetElapsedTimef() + 1) * 0.5;
        float amount3d_ = -ofNoise(position.y / 10, position.x / 10, ofGetElapsedTimef() + 1) * 0.5;

        position.x += amount3d;
        position.y += amount3d_;
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(color);
    if (size > 0.001) {
        ofDrawSphere(position, size);
        ofDrawCircle(position.x, position.y, position.z, size );
    }
}



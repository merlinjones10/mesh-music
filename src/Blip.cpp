#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    position = pos;
    size = 0;
    color.setHsb(ofMap(abs(position.y), 0, 480, 120, 140), 100, 255, 200);
    fallSpeed = ofRandom(0.5, 1.0);
}

void Blip::update(glm::vec3 parentPos){
    if (size >= 5) {
        position = glm::vec3(parentPos.x, parentPos.y, -160);
        color.setHsb(ofMap(abs(position.y), 0, 480, 120, 140) + ofRandom(-5, 5), 100, 255, 200);

    }
    if (size > 0.001 ) {
        color.setSaturation(color.getSaturation() + 4);
        size -=0.05;
        position.z += fallSpeed;
        color.setBrightness(color.getBrightness() - 2.5);
        position.x += ofRandom(-0.5, 0.5);
        position.y += ofRandom(-0.5, 0.5);
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(color);
    if (size > 0.001) {
//        ofDrawSphere(position.x, position.y, -150, size / 2);
        ofDrawCircle(position.x, position.y, position.z, size/ 2);
    }
}



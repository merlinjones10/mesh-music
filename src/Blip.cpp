#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    position = glm::vec3(pos.x, pos.y, 0);
    size = 0;
    color.setHsb(ofRandom(100, 150), 255, 255);
    fallSpeed = ofRandom(0.5, 1.0);
}

void Blip::update(){
    if (size >= 2.0) {
        position.z = 0;
        color.setHsb(ofRandom(120, 140), 255, 255);
        fallSpeed = ofRandom(0.5, 5.0);
    }
    if (size > 0.001 ) {
        size -=0.05;
        position.z += fallSpeed;
        color.setBrightness(color.getBrightness() - 10.0);
        color.setHsb(color.getHue(),color.getSaturation(), color.getBrightness());
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(color);
    if (size > 0.001) {
        ofDrawSphere(position, size);
        ofDrawCircle(position.x, position.y, 1, size / 1.5);

    }
}

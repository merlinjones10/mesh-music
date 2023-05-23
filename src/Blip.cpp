#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    float speed = 1.0;
    position = glm::vec3(pos.x, pos.y, 0);
    size = 0;
    color.setHsb(ofRandom(1, 40), 255, 255);
    particleDirection = ofVec3f(ofRandom(-speed, speed), ofRandom(-speed, speed), ofRandom(-speed, speed));
    active = false;
}

void Blip::update(){
    
    if (active && size > 0.2 ) {
        size -=0.05;
        position += particleDirection;
        color.setBrightness(color.getBrightness() - 5.0);
//        color.setHsb(color.getHue(),color.getSaturation(), color.getBrightness());
    } else if (active) {
        position += particleDirection;
        if (color.getBrightness() > 20) {
            color.setBrightness(color.getBrightness() - 1.0);
        }
        
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(color);
    if (size > 0.001) {
        ofDrawSphere(position, size);
        ofDrawCircle(position.x, position.y, position.z, size / 1.5);
    }
}

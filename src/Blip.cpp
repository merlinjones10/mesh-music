#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    position = pos;
    size = 0;
//    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
    color.setHsb(ofRandom(100, 150), 255, 255);
    fallSpeed = ofRandom(0.5, 1.0);
}

void Blip::update(){
    if (size >= 2.0) {
        color.setHsb(ofRandom(40, 50), 10, 255);
//        position.z -=
        fallSpeed = ofRandom(0.5, 5.0);
    }
    if (size > 0.001 ) {
        size -=0.05;
//        position.z -= fallSpeed;
//        color.setBrightness(color.getBrightness() - 10.0);
//        color.setSaturation(color.getSaturation() + 10.0);
//        color.(color, count);
//        color.setHsb(color.getHue(),color.getSaturation(), color.getBrightness());
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(color);
    if (size > 0.001) {
        ofDrawSphere(position, size);
        ofDrawCircle(position.x, position.y, position.z, size / 1.5);

    }
}
// work on falling star effect
// work on brighness > fade and nice colors

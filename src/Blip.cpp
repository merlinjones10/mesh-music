#include "ofMain.h"
#include "Blip.hpp"

Blip::Blip(glm::vec3 pos) {
    position = pos;
    size = 0;
    color.set(ofRandom(100),ofRandom(100),ofRandom(2));
}

void Blip::update(){
    if (size > 0.001 ) {
        size -=0.05;
        position.z -= 0.1;
    }
}

void Blip::draw(glm::vec3 parentPosition){
    ofSetColor(120);
    ofDrawCircle(position.x, position.y, position.z, size);
}
// work on falling star effect
// work on brighness > fade and nice colors

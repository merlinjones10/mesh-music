#include "ofMain.h"
#include "ofApp.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"
#include "ofxOsc.h"

ofEvent<glm::vec3> NoteBlob::onBlobBangGlobal = ofEvent<glm::vec3>();

NoteBlob::NoteBlob(glm::vec3 pos, vector<int> _speedChoices) : blip(pos) {
    position = pos;
    size = 1;
    direction = glm::vec3(0, 0, 0.1);
    speedChoices = _speedChoices;
//    subDivSpeed = _speedChoices[(int)ofRandom(_speedChoices.size() - 1)];
    subDivSpeed = _speedChoices[4];
    float speed = 0.01 * subDivSpeed;
    color.setHsb(80, ofRandom(200), ofRandom(255), 200);
}


void NoteBlob::update(float speedDampen){
    float boingPoint = BASE_SPEED * 3;
    float speed = speedDampen * subDivSpeed;
    float mappedBrightness = ofMap(speed, 0.01, 0.01*speedChoices.back(), 10, 255);
    if (position.z >= boingPoint) {
        blip.size = 2;
        sendMesg();
        direction = glm::vec3(0, 0, -speed);
        position.z = position.z - speed;
//        subDivSpeed = speedChoices[(int)ofRandom(speedChoices.size() - 1)];
        color.setHsb(ofRandom(70, 100), ofRandom(100), ofRandom(100), 200);

        
    } else if (position.z <= 0) {
//        blip.size = 2;
//        sendMesg();
        position.z = position.z + speed;
        direction = glm::vec3(0, 0, speed);
        subDivSpeed = speedChoices[(int)ofRandom(speedChoices.size() - 1)];
        color.setHsb(ofRandom(100, 120), ofRandom(100), ofRandom(100), 200);

    }
    position += direction;
    blip.update();
}

void NoteBlob::draw(){
    ofSetColor(color);
    blip.draw(position);
    ofSetColor(color);
//    ofDrawSphere(position, size);
}

void NoteBlob::reset(){
    position.z = -1.0;
}

void NoteBlob::sendMesg(){
    ofNotifyEvent(onBlobBangGlobal, position);

//    ofxOscMessage m;
//    m.setAddress("/blip/position");
////
//    m.addIntArg(position.x);
//    m.addIntArg(position.y);
//    oscSender.sendMessage(m, false);

}

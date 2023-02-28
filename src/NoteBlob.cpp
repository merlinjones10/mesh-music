#include "ofMain.h"
#include "ofApp.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"
#include "ofxOsc.h"

ofEvent<glm::vec3> NoteBlob::onBlobBangGlobal = ofEvent<glm::vec3>();

bool rollDice(float probability){
    return ofRandom(100) > probability;
}

vector<int> NoteBlob::s_subDivChoices = {1 ,2 ,4 ,6 ,8 };
int NoteBlob::s_baseDistance = 18;
float NoteBlob::s_tempo = 1.0 / 64.0;

NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    direction = glm::vec3(0, 0, s_tempo);
    subDiv = s_subDivChoices[4]; // start first uniform
    speed = s_tempo * subDiv;
    color.setHsb(80, ofRandom(200), ofRandom(255), 200);
}

void NoteBlob::update(float var1){
    if (position.z >= s_baseDistance) {
        blip.size = 2;
        sendMesg();
        position.z = position.z - speed;
        
        subDiv = s_subDivChoices[(int)ofRandom(s_subDivChoices.size() - 1)];
        speed = s_tempo * subDiv;
        
        if (rollDice(99.5)) {
            speed = ofRandom(0.5, 3.0);
            ofLog() << speed;
        }
        
//        direction = glm::vec3(0, 0, speed);
        direction = glm::vec3(0, 0, -speed);

        color.setHsb(ofRandom(70, 100), ofRandom(100), ofRandom(100), 200);

    } else if (position.z <= 0) {
        position.z = position.z + speed;
        direction = glm::vec3(0, 0, speed);
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
    position.z = (s_baseDistance * 3) - 0.5;
    direction = glm::vec3(0, 0, 1);

}

void NoteBlob::sendMesg(){
    ofNotifyEvent(onBlobBangGlobal, position);
}

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
float NoteBlob::s_tempo = 1.0 / 128.0;

NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    direction = glm::vec3(0, 0, 0);
    subDiv = s_subDivChoices[4];
    speed = s_tempo * subDiv;
    hasBanged = false;
    color.setHsb(100, ofRandom(200), ofRandom(255), 200);
    bang = false;
}

void NoteBlob::update(float noiseValue){
    
    position.z += noiseValue;
    
    ofVec3f p1(position);
    ofVec3f p2(position.x, position.y, 0.0);
    float distance = p1.distance(p2);
    if (distance < 0.1) {
            
        if (!hasBanged) {
                    setBang(true);
            position.y += 400;
            hasBanged = true;
        }
    };
    blip.update();
}

void NoteBlob::draw(){
    blip.draw(position);
    ofDrawSphere(position, size);
}

void NoteBlob::reset(){
    position.z = (s_baseDistance * 3) - 0.5;
    direction = glm::vec3(0, 0, 1);
}

void NoteBlob::setBang(bool value){ // unused param;
    color.setHsb(ofRandom(80, 100), ofRandom(200), ofRandom(255), 200);
    blip.size = 2;
    blip.position.z = 1;
    sendMesg();
}

void NoteBlob::sendMesg(){
    ofNotifyEvent(onBlobBangGlobal, position);
}

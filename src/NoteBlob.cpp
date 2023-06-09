#include "ofMain.h"
#include "ofApp.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"
#include "ofxOsc.h"
#include "State.h"



ofEvent<glm::vec3> NoteBlob::onBlobBangGlobal = ofEvent<glm::vec3>();

bool rollDice(float probability){
    return ofRandom(100) > probability;
}

vector<int> NoteBlob::s_subDivChoices = {1 ,2 ,4 ,6 ,8 };
int NoteBlob::s_baseDistance = 18;
float NoteBlob::s_tempo = 1.0 / 400.0;

NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    direction = glm::vec3(0, 0, s_tempo);
    subDiv = s_subDivChoices[4]; // start first uniform
    speed = s_tempo * subDiv;
    color.setHsb(80, ofRandom(20), ofRandom(255));
    hasBanged = false;
    active = false;
}

void NoteBlob::update(State appState){
    if (position.z > 10 ) {
        blip.size = 5.0;
        sendMesg();
        position.z = 0;
    };
    
    if (ofRandom(1001) > 800 && appState.preset == 2 ) {
        direction = glm::vec3(0, 0, 0);
    } else {
        float amount3d = ofSignedNoise(position.x / appState.params.xParam, position.y / appState.params.yParam, ofGetElapsedTimef() + appState.params.seed) * appState.params.speed;
    //    float amount2d = ofSignedNoise(position.y, ofGetElapsedTimef() + 0) * 0.1;
    //    float amount1d = (ofNoise(ofGetElapsedTimeMillis()) * 0.1) + ofRandom(0.2);

        direction = glm::vec3(0, 0, amount3d);
    }

    position += direction;
    blip.update(position);
}

void NoteBlob::draw(){
//    ofSetColor(color);
    blip.draw(position);
//    ofSetColor(color);
//    ofDrawSphere(position, size);
}

void NoteBlob::reset(){
    position.z = (s_baseDistance * 3) - 0.5;
    direction = glm::vec3(0, 0, 1);

}

void NoteBlob::sendMesg(){
    ofNotifyEvent(onBlobBangGlobal, position);
}

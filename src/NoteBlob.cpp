#include "ofMain.h"
#include "ofApp.h"
#include "NoteBlob.hpp"
#include "Blip.hpp"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 12346

NoteBlob::NoteBlob(glm::vec3 pos) : blip(pos) {
    position = pos;
    size = 1;
    color.set(ofRandom(255),ofRandom(255),ofRandom(255));
//    oscSender.setup(HOST, PORT);
    direction = glm::vec3(0, 0, 0.1);
    subDivSpeed = (int)ofRandom(1, 10) * 2;
    ofLog() << subDivSpeed;
    
}


void NoteBlob::update(float liquidness, float speed){
//    position.z += ofSignedNoise( position.x, ofGetElapsedTimef() ,position.x) * liquidness;
    
    int diceRoll = (int)ofRandom(100);
    int probability = 50;
    float boingPoint = BASE_SPEED * subDivSpeed;
    if (position.z >= boingPoint) {
//        ofLog() << "Bong";
        //        sendMesg();
        direction = glm::vec3(0, 0, -0.1);
        position.z = position.z - 0.1;
        if (diceRoll > probability) {
            subDivSpeed = 1;
        } else {
            subDivSpeed = (int)ofRandom(1, 10) * 2;
        }

        
    } else if (position.z <= 0) {
//        ofLog() << "Bing";
        blip.size = 2;
//        sendMesg();
        position.z = position.z + 0.1;
        direction = glm::vec3(0, 0, 0.1);
        if (diceRoll > probability) {
            subDivSpeed = 1;
        } else {
            subDivSpeed = (int)ofRandom(1, 10) * 2;
        }

    }
    position += direction;
    blip.update();
}

void NoteBlob::draw(){
    ofSetColor(color);
    blip.draw();
    ofNoFill();
//    ofDrawSphere(position, size);
    ofFill();
}

void NoteBlob::reset(){
    position.z = 0; 
}

void NoteBlob::sendMesg(){
//    ofxOscMessage m;
//    m.setAddress("/blip/position");
//
//    m.addIntArg(position.x);
//    m.addIntArg(position.y);
//    oscSender.sendMessage(m, false);
}

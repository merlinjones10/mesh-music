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
    

}


void NoteBlob::update(float liquidness, float speedDampen){
//    position.z += ofSignedNoise( position.x, ofGetElapsedTimef() ,position.x) * liquidness;
    
    float speed = 0.0;
    // the distace is the subdivision..
    // if the blod has to travel twice as far then it will be twice as slow... 2/4
    // if the blob has to travel half as far it will be twice as fast... 1/8
    if (position.z >= 5) {
        ofLog() << "Bong";
        //        sendMesg();

        direction = glm::vec3(0, 0, -0.1);
        
    } else if (position.z <= 0) {
        ofLog() << "Bing";
//        sendMesg();
        direction = glm::vec3(0, 0, 0.1);
        
    }
    position += direction;


    
    if (abs(position.z) <= 0.0) {
        blip.size = 2;
        ofLog() << "Bing";
//        position.z = position.z + position.z;
//        if (position.z > 0) {
//            position.z = 3;
//        } else {
//            position.z = -3;
//        }
    }
//    blip.update();
}

void NoteBlob::draw(){
    ofSetColor(color);
//    blip.draw();
    ofNoFill();
//    ofSetSphereResolution(3);
    ofDrawSphere(position, size);
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

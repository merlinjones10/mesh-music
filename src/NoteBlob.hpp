#include "ofMain.h"
#include "Blip.hpp"
#include "ofxOsc.h"


#ifndef NoteBlob_hpp
#define NoteBlob_hpp

class NoteBlob {
public:

    glm::vec3 position;
    int size;
    ofColor color;
    Blip blip;
    ofxOscSender oscSender;


    void update(float, float);
    void draw();
    void reset();
    void sendMesg();
    
    NoteBlob(glm::vec3); 
    
private:
};


#include <stdio.h>

#endif /* NoteBlob_hpp */

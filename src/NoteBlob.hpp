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
    glm::vec3 direction;
    vector<int> speedChoices;
    int subDivSpeed; 
    void update(float);
    void draw();
    void reset();
    void sendMesg();
    
    static ofEvent<glm::vec3> onBlobBangGlobal;

    NoteBlob(glm::vec3, vector<int>);
    
private:
};


#include <stdio.h>

#endif /* NoteBlob_hpp */

#include "ofMain.h"
#include "Blip.hpp"
#include "ofxOsc.h"


#ifndef NoteBlob_hpp
#define NoteBlob_hpp

class NoteBlob {
public:
    NoteBlob(glm::vec3);
    Blip blip;

    ofColor color;
    bool bang; 
    glm::vec3 position;
    glm::vec3 direction;
    
    int size;
    int subDiv;
    float speed;
    bool hasBanged; 

    void update(float);
    void draw();
    void reset();
    void sendMesg();
    void setBang(bool);
    static ofEvent<glm::vec3> onBlobBangGlobal;
    static vector<int> s_subDivChoices;
    static int s_baseDistance;
    static float s_tempo;

    
private:
};


#include <stdio.h>

#endif /* NoteBlob_hpp */

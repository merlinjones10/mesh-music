#include "ofMain.h"
#include "Blip.hpp"

#ifndef NoteBlob_hpp
#define NoteBlob_hpp

class NoteBlob {
public:

    glm::vec3 position;
    int size;
    ofColor color;
    Blip blip;

    void update(float, float);
    void draw();
    void reset();
    
    NoteBlob(glm::vec3); // constructor
//    ~Particle(); // destructor
    
private:
};


#include <stdio.h>

#endif /* NoteBlob_hpp */

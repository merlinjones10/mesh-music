#include "ofMain.h"


#ifndef NoteBlob_hpp
#define NoteBlob_hpp

class NoteBlob {
public:

    glm::vec3 position;
    int size;
    ofColor color;
    void update();
    void draw();
    void reset();
    
    NoteBlob(glm::vec3); // constructor
//    ~Particle(); // destructor
    
private:
    
};


#include <stdio.h>

#endif /* NoteBlob_hpp */

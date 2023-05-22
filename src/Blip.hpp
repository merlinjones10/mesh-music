#ifndef Blip_hpp
#define Blip_hpp

#include "ofMain.h"

class Blip {
public:
    glm::vec3 position;
    float size;
    ofColor color;
    ofVec3f particleDirection; 
    void setup();
    void update();
    void draw(glm::vec3);
    void reset();
    
    Blip(glm::vec3);
    
private:
};

#include <stdio.h>

#endif /* Blip_hpp */

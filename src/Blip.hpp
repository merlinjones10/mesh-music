#ifndef Blip_hpp
#define Blip_hpp

#include "ofMain.h"

class Blip {
public:
    glm::vec3 position;
    float size;
    ofColor color;
    void setup();
    void update();
    void draw();
    void reset();
    
    Blip(glm::vec3);
    
private:
};

#include <stdio.h>

#endif /* Blip_hpp */

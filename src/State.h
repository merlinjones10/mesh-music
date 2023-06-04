//
//  State.hpp
//  mesh_music
//
//  Created by Merlin Jones on 04/06/2023.
//
#include "ofMain.h"

#ifndef State_hpp
#define State_hpp
struct perlinParams {
    int xParam;
    int yParam;
    int seed;
    float speed;
} ;

class State {
public:
    State();
    int preset;
//    int speed;
    void stop();
    void setPerlinParams(int);
    void printState();
    perlinParams params;

private:
};

#include <stdio.h>

#endif /* State_hpp */

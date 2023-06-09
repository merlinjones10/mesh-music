#include "State.h"


State::State() {
    ofLog() << "Setting state";
    setPerlinParams(1);
}
void State::setPerlinParams(int preset) {
    switch (preset) {
        case 1:
            params.xParam = 1000;
            params.yParam = 1000;
            params.seed = ofRandom(10000);
            params.speed = 1;
            break;
        case 2:
            params.xParam = 100;
            params.yParam = 150000;
            params.seed = ofRandom(10000);
            params.speed = 0.05;
            break;
        case 3:
            params.xParam = 150;
            params.yParam = 10;
            params.seed = ofRandom(10000);
            params.speed = 1.8;
            break;
        case 0:
            params.xParam = 0;
            params.yParam = 0;
            params.seed = 0;
            params.speed = 0;
            break;
    }
};

void State::printState() {
    ofLog() << "x:" << params.xParam << " - " << "y: " << params.yParam << '\n';
};

void State::stop() {
    setPerlinParams(0);
}

//void setHue() {
//
//}

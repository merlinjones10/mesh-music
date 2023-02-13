#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetVerticalSync(true);

    // load an image from disk
    img.load("bach.png");

    // we're going to load a ton of points into an ofMesh
    mesh.setMode(OF_PRIMITIVE_POINTS);
    noteMesh.setMode(OF_PRIMITIVE_POINTS);
    // loop through the image in the x and y axes
    int skip = 2; // load a subset of the points
    for(int y = 0; y < img.getHeight(); y += skip) {
        for(int x = 0; x < img.getWidth() -1; x += skip) {
            ofColor cur = img.getColor(x, y);
            if(cur.getBrightness() < 10) {
                float z = ofMap(cur.a, 0, 255, -10, 10);
                cur.a = 255;
                noteMesh.addColor(cur);
                glm::vec3 pos(x, y, z);
                noteMesh.addVertex(pos);
            }
            else {
                float z = 0.0;
                cur.a == 255;
                mesh.addColor(ofColor(ofRandom(100, 110), ofRandom(50, 60), ofRandom(200, 255)));
                glm::vec3 pos(x, y, z);
                mesh.addVertex(pos);
            }
        }
    }

    ofEnableDepthTest();
//    glEnable(GL_POINT_SMOOTH); // use circular points instead of square points
    glPointSize(4); // make the points bigger
}

//--------------------------------------------------------------
void ofApp::update() {
    float liquidness = 5;
    float amplitude = mouseY/100.0;
    float speedDampen = 20;
    auto &verts = noteMesh.getVertices();

    for(unsigned int i = 0; i < verts.size(); i++){
//        verts[i].x += ofSignedNoise(verts[i].x/liquidness, verts[i].y/liquidness,verts[i].z/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
//        verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
//        verts[i].z += ofNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
        verts[i].z = ofMap(ofNoise(i, (ofGetElapsedTimef() * 0.5)), 0, 1, -9, 9) ;

    }

}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackgroundGradient(ofColor::gray, ofColor::black, OF_GRADIENT_CIRCULAR);

    // even points can overlap with each other, let's avoid that
    cam.begin();
    ofScale(2, -2, 2); // flip the y axis and zoom in a bit
    ofRotateYDeg(90);
    ofTranslate(-img.getWidth() / 2, -img.getHeight() / 2);
    mesh.draw();
    noteMesh.draw();
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}


#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
#ifdef __APPLE__
    ofSetOpenGLVersion(3, 3);
#else
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
#endif
	ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}

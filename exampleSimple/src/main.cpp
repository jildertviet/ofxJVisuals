#include "ofApp.h"

int main(){
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:

	// ofSetupOpenGL(1920, 1080, OF_WINDOW);

	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
	settings.setSize(1920, 1080);
	// settings.height = 1080;
	ofCreateWindow(settings);


	ofRunApp(new ofApp());
}

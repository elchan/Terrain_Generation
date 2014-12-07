#pragma once

#include "ofMain.h"
#include "Terrain.hpp"

#ifdef __APPLE__
#include "ofxGameCamera.h"
#include "ofxSecondWindow.h"

#endif

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void diamondSquare(ofMesh &mesh, ofImage &img);
		void genTerrain();
  
  void renderTerrain();


public:
		ofImage landImg;  
		ofImage waterImg;
		ofVec3f previous, current, target;
		ofEasyCam easyCam;		
		ofCamera fullCamera;
		deque<ofVec3f> pathVertices;
		ofMesh pathLines;
		ofMesh m_mesh;
		ofMesh m_terrain;
#ifdef __APPLE__
    ofxGameCamera camera1;
#else
		ofCamera camera1;
#endif
		ofCamera camera2;
//		ofShader shader;
		ofShader model_shader;
		ofShader terrain_shader;
		Terrain terrain;
		float scale;
		float defaultHeight;
		ofTexture myTexture;
		bool topView;
		bool drawShip;
		ofIcoSpherePrimitive sphere;
		ofTrueTypeFont fontObj;
		bool info;
		bool shipShade;
		ofLight light1;
		ofVec3f light1_current_position;
		ofLight light2;
		ofLight light3;
		ofIcoSpherePrimitive ligthBulb;
		int tess;
//		ofMesh m_mesh;
//		ofMesh m_terrain;
		ofVbo m_vbo;
		ofVboMesh m_vbomesh;
		GLint tessellatemode;
		GLint tessLevel;
		GLfloat nearClip;
		GLfloat farClip;
		GLfloat imgPlane;
		bool wireframemode;
		int mode;
		int smooth;
		float angle;
		ofShader shader;
		ofImage image;
	    ofImage imageMask;
	    ofFbo fbo;
	    ofFbo maskFbo;
		int fboWidth;
		int fboHeight;
		float camFOV;
<<<<<<< HEAD
		float cam1FOV;
		float cam2FOV;
=======
  
    ofRectangle rect;
>>>>>>> 645f7668009d94be2c9d0f82f161366be109bb00

#ifdef __APPLE__
  
ofxSecondWindow secondWindow;
  
#endif

};

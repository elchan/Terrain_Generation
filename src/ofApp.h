#pragma once

#include "ofMain.h"
#include "Terrain.hpp"
#include "ofxSecondWindow.h"

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
		ofVec3f previous, current, target;
		ofEasyCam easyCam;		
		ofCamera fullCamera;
		deque<ofVec3f> pathVertices;
		ofMesh pathLines;
		ofMesh m_mesh;
		ofMesh m_terrain;
		ofCamera camera1;
		ofCamera camera2;
//		ofShader shader;
		ofShader model_shader;
		ofShader terrain_shader;
		Terrain terrain;
		float scale;
		float defaultHeight;
		ofImage myImage;
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
	    ofxSecondWindow secondWindow;

};

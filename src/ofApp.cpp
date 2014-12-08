#include "ofApp.h"

/*
	rotateToNormal will rotate everything using ofRotate. the rotation amount
	and axis are generated using an ofQuaternion. the trick is to use ofQuaternion
	to determine the rotation that is required from a standard axis (0,0,1) to the
	desired normal vector, then apply that rotation. 
*/
//--------------------------------------------------------------
void rotateToNormal(ofVec3f normal) {
	normal.normalize();
	
	float rotationAmount;
	ofVec3f rotationAngle;
	ofQuaternion rotation;
	
	ofVec3f axis(0, 0, 1);
	rotation.makeRotate(axis, normal);
	rotation.getRotate(rotationAmount, rotationAngle);
	ofRotate(rotationAmount, rotationAngle.x, rotationAngle.y, rotationAngle.z);
}

//--------------------------------------------------------------


void ofApp::setup(){

	float x = 0.;
float y = 1.;

		fprintf(stderr, "Tan %f %f %f\n", 0,0,tan(x/y));
		exit();

#ifdef TARGET_OPENGLES
	shader.load("shadersES2/shader");
#else
	if(ofIsGLProgrammableRenderer()){
		shader.load("shadersGL3/shader");
	}else{
		shader.load("shadersGL2/shader");
	}
#endif

#ifdef __APPLE__
  secondWindow.setup("second window", 50, 50, 512, 384, false);
  camera1.setup();
#endif
	fboWidth = 256;
	fboHeight = 256;
	angle = 0;
	smooth = 0;
	mode = 0;
	info = false;
	shipShade = false;
	fontObj.loadFont("C:\\Users\\Ellison\\Documents\\Models\\orange juice 2.0.ttf", 32);
	defaultHeight = 100;
	drawShip = true;
	sphere.setRadius( 20 );
	scale = 3;
	tess = 1;
	topView = true;
	ofSetVerticalSync(true);
	wireframemode = false;
	camFOV = 0;
	// draw the vertices in pathLines as a line strip
	pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
	genTerrain();
//	m_mesh.load("C:\\Users\\Ellison\\Documents\\Models\\bsg.ply");
//	m_terrain.load("NewTerrain.ply");
	//m_terrain.load("Terrain.ply");
	  
//	shader.load("shader.vert", "shader.frag");
//	model_shader.load("modelshader.vert", "modelshader.frag");
//	terrain_shader.load("vt.vertexshader", "frag.fragmentshader"/*,"geo.geometryshader" */);
	terrain_shader.load("shader.vert", "shader.frag", "geo.geometryshader");
//	terrain_shader.load("shader.vert", "shader.frag");

	tessLevel = 0;
	//texture
	landImg.loadImage("texture.jpg");
	waterImg.loadImage ( "water.jpg");

	//myTexture =landImg.getTextureReference();

	light1_current_position = ofVec3f( 10.f ,0.f , 0.f );

	current.x = 0;
	current.y = -275;
	current.z = 20;//terrain.maxHeight + defaultHeight;;

	camera1.setGlobalPosition((ofVec3f(current.x, current.y, current.z)));
	camera1.lookAt(ofVec3f(0, 0, 0));
	target = ofVec3f(0,0,0);
  
	camera2.setGlobalPosition((ofVec3f(current.x, current.y, 800)));
	camera2.lookAt(ofVec3f(0, 0, 0));
   	ofSetVerticalSync(true);
	ofEnableDepthTest();
	easyCam.setDistance(100);
    image.loadImage("img.jpg");
    imageMask.loadImage("mask.jpg");
    fbo.allocate(fboWidth, fboHeight);
    maskFbo.allocate(fboWidth, fboHeight);

	mainCam = camera1;
	insetCam = camera2;
}

void ofApp::genTerrain()
{
	for (int i=0; i< 6; i++)
	{
		terrain.diamondSquareIterationByIdx();
	}
}

float convertRGB2Gray(ofColor color)
{
	float gray = 0.0f;

	gray = (color.r+color.g+color.b)/2;

	return (255-gray)/10;
}


//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (topView)
	{
		mainCam = camera1;
		insetCam = camera2;
	}
	else
	{
		mainCam = camera2;
		insetCam = camera1;
	}

	glEnable(GL_CULL_FACE);
	glShadeModel ( GL_SMOOTH ) ;
	glFrontFace(GL_CW);
	//glEnable(GL_COLOR_MATERIAL) ;

	
	if (info)
	{
		char str[255];
		sprintf(str, "x=%2.0f y=%2.0f z=%2.0f frame rate: %2.0f\n\0",current.x, current.y, current.z, ofGetFrameRate());
		fontObj.drawString (str,0, terrain.maxHeight);
	}
	mainCam.begin();
	renderTerrain(mainCam, "mainCam");
	mainCam.end();
  /*
    maskFbo.begin();

    ofClear(255, 0, 0, 255);
    
    float imageMaskX = mouseX / (float)ofGetWidth();
    imageMaskX = ofClamp(imageMaskX, 0, 1);
    imageMaskX = -(imageMask.getWidth() - maskFbo.getWidth()) * imageMaskX;
    imageMask.draw(imageMaskX, 0);
    
    maskFbo.end();
	*/

#ifdef __APPLE__
  secondWindow.begin();
#else
  fbo.begin();
  glDisable(GL_CULL_FACE);
#endif
	ofClear(128,128,128);
	insetCam.begin();
	bool originalWireFrame = wireframemode;
	wireframemode = true;
	renderTerrain(insetCam, "insetCam");
	insetCam.end();
	wireframemode = originalWireFrame;
#ifdef __APPLE__
  secondWindow.end();
#else
    //maskFbo.draw(0,0);
	glEnable(GL_CULL_FACE);
	fbo.end();
	//ofRotateZ(180);
	fbo.draw(1024-fboWidth,768-fboHeight, fboWidth,fboHeight);
#endif
}

void ofApp::renderTerrain(ofCamera cam, string camName) {

	ofMatrix4x4 cameraViewMatrix;
	cameraViewMatrix.makeLookAtViewMatrix(cam.getPosition() , cam.getLookAtDir() , ofVec3f( 0.f ,1.f , 0.f));

  light1.enable();
  
	if ( angle >= 2*3.14159) angle = 0;
//	light1.setPosition(0, 180*abs(cos(angle+=0.001)), terrain.maxHeight*3*abs(sin(angle+=0.001)));
	angle = 0;
	light1.setPosition(0, 0, terrain.maxHeight*8);
  
	light1.setDiffuseColor(ofFloatColor(1.f , 1.f , 1.f));
	light1.setAttenuation(0.f , 0.f, 0.f );
  
	//a sphere as the light source
	ligthBulb.setPosition (light1.getPosition());
	ligthBulb.setRadius(10.f);
	ligthBulb.enableColors();
	ligthBulb.draw();
  
  
	myTexture.bind();
	terrain_shader.begin();
	
	terrain_shader.setUniformTexture("tex0", landImg, 1);
	terrain_shader.setUniformTexture("tex1", waterImg, 2);

  
	terrain_shader.setUniform1f("maxHeight", terrain.maxHeight);
	terrain_shader.setUniform1f("minHeight", terrain.minHeight);
	terrain_shader.setUniform1f("scale", scale);
	terrain_shader.setUniform1i("tess", tess);
	terrain_shader.setUniform1i("tessLevel", tessLevel);
	terrain_shader.setUniform1f("far",cam.getFarClip());
	terrain_shader.setUniform1f("near",cam.getNearClip());
	terrain_shader.setUniform1f("imgPlane",cam.getImagePlaneDistance());
	terrain_shader.setUniform1i("poly", smooth);
	terrain_shader.setUniform1i("maxX", terrain.maxX);	
	if (camName == "insetCam")
	{
		terrain_shader.setUniform1i("camNam", 2);	
	}
	else
	{
		terrain_shader.setUniform1i("camNam", 1);	
	}
	float position[3];
	ofVec3f posvec = mainCam.getPosition();
	position[0] = posvec.x;
	position[1] = posvec.y;
	position[2] = posvec.z;
	terrain_shader.setUniform3fv("camLoc",position,1);
	terrain_shader.setUniform1f("camFOV", cam.getFov()); 
	terrain_shader.setUniformMatrix4f("modelViewMatrixMainCam",cameraViewMatrix);
	//terrain_shader.setUniform1f("camFOV", 180); 
	//fprintf(stderr,"FOV: %f\n", camera1.getFov());
  
	//Light
	terrain_shader.setUniform3f ("LightPosition",light1.getPosition().x, light1.getPosition().y ,light1.getPosition().z); 
	terrain.draw(wireframemode);  
	terrain_shader.end();
	myTexture.unbind();
	light1.disable();
}

void setCamPos(ofCamera &cam, ofVec4f posInc)
{
	auto pos = cam.getPosition();
	pos.x += posInc.x;
	pos.y += posInc.y;
	pos.z += posInc.z;
	cam.setPosition(pos);
}

void rotCam(ofCamera &cam, float deg)
{
	cam.rotate(deg, 0, 0, 1);
}

void incrFOV(ofCamera &cam, float incr)
{
	float fov = cam.getFov();
	fov+= incr;
	cam.setFov(fov);
}

void ofApp::keyPressed(int key){
#ifdef __APPLE__
  if (key == '0') {
    tessLevel = 0;
  }
  else if (key == '1') {
    tessLevel = 1;
  }
  else if (key == '2') {
    tessLevel = 2;
  }
  else if (key == '3') {
    tessLevel = 3;
  }
  else if (key == '4') {
    tessLevel = 4;
  }
#else
	float posInc = 10.0f;
	switch (key)
	{
		case OF_KEY_RIGHT:
			{
				setCamPos(camera1, ofVec4f(posInc,0,0,0));
				setCamPos(camera2, ofVec4f(posInc,0,0,0));
				break;
			}
		case OF_KEY_LEFT:
			{
				setCamPos(camera1, ofVec4f(-posInc,0,0,0));
				setCamPos(camera2, ofVec4f(-posInc,0,0,0));
				break;
			}
		case OF_KEY_UP:
			{
				setCamPos(camera1, ofVec4f(0,+posInc,0,0));
				setCamPos(camera2, ofVec4f(0,+posInc,0,0));
				break;
			}
		case OF_KEY_DOWN:
			{
				setCamPos(camera1, ofVec4f(0,-posInc,0,0));
				setCamPos(camera2, ofVec4f(0,-posInc,0,0));
				break;
			}
		case OF_KEY_PAGE_UP:
			{
				setCamPos(camera1, ofVec4f(0,0,posInc,0));
				setCamPos(camera2, ofVec4f(0,0,posInc,0));
				break;
			}
		case OF_KEY_PAGE_DOWN:
			{
				setCamPos(camera1, ofVec4f(0,0,-posInc,0));
				setCamPos(camera2, ofVec4f(0,0,-posInc,0));
				break;
			}
		case OF_KEY_HOME :
			{
				rotCam(camera1, 10);
				rotCam(camera2, 10);
				break;
			}

		case OF_KEY_END :
			{
				rotCam(camera1, -10);
				rotCam(camera2, -10);
				break;
			}

		case 'w':
			{
				string wfmode ="";
				terrain.setMode();
				if (wireframemode == false)
				{
					wireframemode = true;
					wfmode = "Wireframe ON";
				}
				else
				{
					wireframemode = false;
					wfmode = "Wireframe OFF";
				}
				fprintf(stderr, "%s\n", wfmode.c_str());
			}
			break;
		case 't':
			if (topView)
			{
				topView = false;
				//mainCam = camera2;
				//insetCam = camera1;
				fprintf(stderr,"Top View.\n");
			}
			else
			{
				topView = true;
				//mainCam = camera1;
				//insetCam = camera2;
				fprintf(stderr,"Perspective View.\n");
			}
			break;
		case 'n':
			target.y+=posInc;
			fprintf(stderr, "Target Y: %2.4f\n", target.y);
			break;
		case 'm':
			target.y-=posInc;
			fprintf(stderr, "Target Y: %2.4f\n", target.y);
			break;
		case 'r':
    
			terrain.reset();
			genTerrain();
			m_terrain.load("NewTerrain.ply");
			fprintf(stderr, "Terrain reset\n");
			break;
		case 's':
			if (smooth == 0)
			{
				smooth = 1;
				fprintf(stderr, "Smooth ON\n");
			}
			else
			{
				smooth = 0;
				fprintf(stderr, "Smooth OFF\n");
			}
			break;
		case 'i':
			info = !info;
			break;
		case 'p':
			shipShade = !shipShade;
			break;
		case 'e':
			if (tess == 1)
			{
				tess = 0;
				fprintf(stderr, "Tessellation OFF\n");
			}
			else
			{
				tess = 1;
				fprintf(stderr, "Tessellation ON\n");
			}
			break;
			/*
		case '0':
			mode = 0;
			break;

		case '1':
			mode = 1;
			break;
			*/
		case '0':
			tessLevel = 0;
			fprintf(stderr, "tessLevel= %i\n",tessLevel);
			break;
		case '1':
			tessLevel = 1;
			fprintf(stderr, "tessLevel= %i\n",tessLevel);
			break;
		case '2':
			tessLevel = 2;
			fprintf(stderr, "tessLevel= %i\n",tessLevel);
			break;
		case '3':
			tessLevel = 3;
			fprintf(stderr, "tessLevel= %i\n",tessLevel);
			break;
		case '4':
			tessLevel = 4;
			fprintf(stderr, "tessLevel= %i\n",tessLevel);
			break;
		case 'x':
			{
				float incr = 1.;
				incrFOV(camera1, incr);
				incrFOV(camera2, incr);
				fprintf(stderr, "Cam1:%f Cam2:%f\n",camera1.getFov(), camera2.getFov());
			}
			break;
		case 'z':
			{
				float incr = -1.;
				incrFOV(camera1, incr);
				incrFOV(camera2, incr);
				fprintf(stderr, "Cam1:%f Cam2:%f\n",camera1.getFov(), camera2.getFov());
			}
			break;
			break;


	}
#endif
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
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

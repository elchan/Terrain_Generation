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
	
    secondWindow.setup("second window", 50, 50, 1024, 768, false);
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
	//myTexture =landImg.getTextureReference();

	light1_current_position = ofVec3f( 10.f ,0.f , 0.f );

	current.x = 0;
	current.y = -275;
	current.z = 20;//terrain.maxHeight + defaultHeight;;

	camera1.setGlobalPosition((ofVec3f(current.x, current.y, current.z)));
	camera1.lookAt(ofVec3f(0, 0, 0));
	target = ofVec3f(0,0,0);
  
  camera2.setGlobalPosition(ofVec3f(0, 0, 500));
  camera2.lookAt(ofVec3f(0, 0, 0));
	//myImage.loadImage("C:\\Users\\Ellison\\Pictures\\saj.jpg");  
	

   	ofSetVerticalSync(true);
	ofEnableDepthTest();
	easyCam.setDistance(100);

  /**
    terrain.reset();
	//terrain.diamondSquareIterationByIdx();
	for (auto & vertex : terrain.mesh.getVertices()) {
		vertex.z = 0;
	}
    
	for (auto & vertex : terrain.mesh.getVertices()) {
		terrain.mesh.addNormal(ofVec3f(0, 0, 1));
	}
	//terrain.mesh.enableNormals();
    //m_terrain = terrain.mesh;
	//terrain.diamondSquareIterationByIdx();
    //diamondSquare(m_terrain, landImg);
   */
}

void ofApp::genTerrain()
{
	for (int i=0; i< 4; i++)
	{
		terrain.diamondSquareIterationByIdx();
	}

//	terrain.save();
}

/*
void setMaxMinHeight(ofMesh mesh, float &minH, float &maxH)
{
	vector<ofPoint> verts = mesh.getVertices();

	float minVal = 99999.0;
	float maxVal = 0.0;

	for_each( verts.begin(), verts.end(), [](ofPoint pt)
	{
		if (pt.z > maxVal)
		{
		}
	});
}
*/

float convertRGB2Gray(ofColor color)
{
	float gray = 0.0f;

	gray = (color.r+color.g+color.b)/2;

	return (255-gray)/10;
}

void ofApp::diamondSquare(ofMesh &mesh, ofImage &img)
{
	int width = img.width-1;
	int height = img.height-1;
	int scale = 1;
	
	for (int y=0; y<height ; y+=1)
		for (int x =0 ; x<width; x+=1){
			//left
			mesh.addVertex(ofPoint(x,y,convertRGB2Gray(img.getColor(x,y+0)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//right
			mesh.addVertex(ofPoint(x-scale,y,convertRGB2Gray(img.getColor(x+1,y+0)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//bottom right
			mesh.addVertex(ofPoint(x+scale,y+scale,convertRGB2Gray(img.getColor(x+1,y+1)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//bottom left
			mesh.addVertex(ofPoint(x,y+scale,convertRGB2Gray(img.getColor(x,y+1)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
		}		

		
/*		
	for (int y=-height/2; y<= 0; y+=1)
		for (int x = width/2; x<=0; x+=1){
			//left
			mesh.addVertex(ofPoint(x,y+0,convertRGB2Gray(myImage.getColor(x,y+0)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//right
			mesh.addVertex(ofPoint(x+1,y+0,convertRGB2Gray(myImage.getColor(x+1,y+0)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//bottom right
			mesh.addVertex(ofPoint(x+1,y+1,convertRGB2Gray(myImage.getColor(x+1,y+1)))); 
			mesh.addColor(ofFloatColor(1,1,1)); 
			//bottom left
			mesh.addVertex(ofPoint(x,y+1,convertRGB2Gray(myImage.getColor(x,y+1)))); 
			mesh.addColor(ofFloatColor(1,y+1,1)); 
		}		
		*/
		

	//mesh.load("C:\\Users\\Ellison\\Documents\\Models\\eyes.ply");
	//vbo.setMesh(mesh, GL_STATIC_DRAW);

}

//--------------------------------------------------------------
void ofApp::update(){
	/*
	current.x = 0;
	current.y = 0;
	current.z = 45;
	*/
	//previous = current;
	//
	//
	//// generate a noisy 3d position over time 
	//
	//float t = (2 + ofGetElapsedTimef()) * .1;
	///*
	//current.x = ofSignedNoise(t, 0, 0);
	//current.y = ofSignedNoise(0, t, 0);
	//current.z = ofSignedNoise(0, 0, 5);
	//current *= 550*scale; // scale from -1,+1 range to -400,+400
	//*/
	//
	//
	///*
	//current.x+=t;
	//current *= 550*scale; // scale from -1,+1 range to -400,+400
	//*/

	//if (abs(current.x) > 550*scale) current.x = 0;

	//// add the current position to the pathVertices deque
	//pathVertices.push_back(current);
	//// if we have too many vertices in the deque, get rid of the oldest ones
	//while(pathVertices.size() > 200) {
	//	pathVertices.pop_front();
	//}
	// clear the pathLines ofMesh from any old vertices
	//pathLines.clear();
	//// add all the vertices from pathVertices
	//for(unsigned int i = 0; i < pathVertices.size(); i++) {
	//	pathLines.addVertex(pathVertices[i]);
	//}
}

//--------------------------------------------------------------
void ofApp::draw(){

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
	float zoff = 0.1;
	ofColor cyan = ofColor::fromHex(0x00abec);
	ofColor blue = ofColor::fromHex(0x00abec);
	ofColor magenta = ofColor::fromHex(0xec008c);
	ofColor yellow = ofColor::fromHex(0xffee00);

//	ofBackgroundGradient(cyan * 1.0, cyan * .4);
//	ofNoFill();

	//camera1.setVFlip(true);
//	camera1.setPosition((ofVec3f(0, -275, 140)));

	//camera1.lookAt(ofVec3f(current.x, current.y+target.y, current.z+(zoff)));

	camera1.begin();
	//ofTranslate(current.x, current.y, current.z);
	//glPolygonMode(GL_FRONT, GL_FILL);
	//myTexture.unbind();  
  renderTerrain();
	//ofPushMatrix();
	//ofTranslate(ofGetWidth() /2 , ofGetHeight() /2 , 0 ) ;
	
	

//    light2.disable();
//    light3.disable();
	camera1.end();
  
  
  secondWindow.begin();
  ofClear(1,1,1);
	camera2.begin();
  bool originalWireFrame = wireframemode;
  wireframemode = true;
  renderTerrain();
	camera2.end();
  wireframemode = originalWireFrame;
  secondWindow.end();
}

void ofApp::renderTerrain() {
  light1.enable();
  
	if ( angle >= 2*3.14159) angle = 0;
	light1.setPosition(0, 180*abs(cos(angle+=0.001)), terrain.maxHeight*3*abs(sin(angle+=0.001)));
  
  // light1.setGlobalPosition(light1_current_position);
	light1.setDiffuseColor(ofFloatColor(1.f , 1.f , 1.f));
	light1.setAttenuation(0.f , 0.f, 0.f );
  
	//a sphere as the light source
	ligthBulb.setPosition (light1.getPosition());
	ligthBulb.setRadius(10.f);
	ligthBulb.enableColors();
	ligthBulb.draw();
  
	ofMatrix4x4 cameraViewMatrix;
	cameraViewMatrix.makeLookAtViewMatrix(camera1.getPosition() , camera1.getLookAtDir() , ofVec3f( 0.f ,1.f , 0.f));
//  ofMatrix4x4 tessCamMVP = cameraViewMatrix * camera1.getProjectionMatrix();
	
	//landImg.bind();
	
  
  myTexture.bind();
	terrain_shader.begin();
	
	terrain_shader.setUniformTexture("tex0", landImg, 1);
  
  terrain_shader.setUniform1f("maxHeight", terrain.maxHeight);
  terrain_shader.setUniform1f("minHeight", terrain.minHeight);
  terrain_shader.setUniform1f("scale", scale);
  terrain_shader.setUniform1i("tess", tess);
	terrain_shader.setUniform1i("tessLevel", tessLevel);
	terrain_shader.setUniform1f("far",camera1.getFarClip());
	terrain_shader.setUniform1f("near",camera1.getNearClip());
	terrain_shader.setUniform1f("imgPlane",camera1.getImagePlaneDistance());
	terrain_shader.setUniform1i("poly", smooth);
//  terrain_shader.setUniformMatrix4f("tesselationCamMVP", camera1.getProjectionMatrix());
	float position[3];
	ofVec3f posvec = camera1.getPosition();
	position[0] = posvec.x;
	position[1] = posvec.y;
	position[2] = posvec.z;
	terrain_shader.setUniform3fv("camLoc",position,1);
  
  
  
	//Light
	terrain_shader.setUniform3f ("LightPosition",light1.getPosition().x, light1.getPosition().y ,light1.getPosition().z);
  
  //	ofScale(scale,1.0f,scale);
	//ofRotateX(90);
	//ofTranslate(0,0,0);
	//ofRotateY(180);
	//terrain.draw();
	myImage.bind();
  terrain.draw(wireframemode);
  myImage.unbind();
  
  //	if (wireframemode)
  //	{
  //		m_terrain.drawWireframe();
  //	}
  //	else
  //	{
  //		m_terrain.draw();
  //	}
	//m_terrain.drawWireframe();
  
	terrain_shader.end();
	myTexture.unbind();
	//ofPopMatrix();
	//ofRotateX(15);
  
	//ofSetColor(255);
	//ofDrawGrid(500, 10, false, false, true, false);
	
	/*
   // draw the path of the box
   ofSetLineWidth(2);
   ofSetColor(cyan);
   pathLines.draw();
   */
	// draw a line connecting the box to the grid
	//ofSetColor(yellow);
	//ofLine(current.x, current.y, current.z, current.x, 0, current.z);
	
	
  //if( (current - previous ).length() > 0.0 )
	{
    //rotateToNormal(current - previous);
		//easyCam.setTarget(current + (550*scale,550*scale,-15));
		//ofVec3f currentRelative2Drone = current-(current-m_mesh.getCentroid());
		//easyCam.enableOrtho();
		//easyCam.setFarClip(1000);
		//easyCam.setNearClip(550);
    //		camera1.setPosition(ofVec3f(current.x,current.y, current.z));
    //		camera1.lookAt(ofVec3f(current.x,current.y, current.z));
		//easyCam.setDistance(current.y);
		//easyCam.setTarget(ofVec3f(current.x,current.y, current.z));
		//ofNode targ = easyCam.getTarget();
		//fprintf(stderr, "Top View %2.4f %2.4f %2.4f\n", targ.getPosition().x, targ.getPosition().y, targ.getPosition().z);
		//ofRotateY(90);
		//ofRotateX(90);
		//sphere.setPosition((ofVec3f(current.x, current.y+target.y, current.z+1)));
 		//sphere.draw();
    if (drawShip)
    {
      
      
      
      
      
      //                ofSetColor(255,0,0);
      //                if (shipShade)
      //                {
      //                    ofTranslate((ofVec3f(current.x, current.y+target.y, current.z+(zoff))));
      //                    model_shader.begin();
      //                    ofScale (10,10,10);
      //                    m_mesh.draw();
      //                    model_shader.end();
      //                }
      //                else
      //                {
      //                    ofTranslate((ofVec3f(current.x, current.y+target.y, current.z+(zoff))));
      //                    ofScale (10,10,10);
      //                    m_mesh.drawWireframe();
      //                }
      
      
      
    }
  }
  
  light1.disable();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	float posInc = 10.0f;
	switch (key)
	{
		case OF_KEY_RIGHT:
			{
				auto pos = camera1.getPosition();
				pos.x+=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_LEFT:
			{
				auto pos = camera1.getPosition();
				pos.x-=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_UP:
			{
				auto pos = camera1.getPosition();
				pos.y+=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_DOWN:
			{
				auto pos = camera1.getPosition();
				pos.y-=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_PAGE_UP:
			{
				auto pos = camera1.getPosition();
				pos.z+=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_PAGE_DOWN:
			{
				auto pos = camera1.getPosition();
				pos.z-=posInc;
				camera1.setPosition(pos);
				break;
			}
		case OF_KEY_HOME :
			{
				camera1.rotate(10, 0, 0, 1);
				break;
			}

		case OF_KEY_END :
			{
				camera1.rotate(-10, 0, 0, 1);
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
				target.y=terrain.minHeight;
				current.y= scale*550;
				fprintf(stderr,"Top View.\n");
			}
			else
			{
				target.y=0;
				current.y= terrain.maxHeight+defaultHeight;
				fprintf(stderr,"Ortho View.\n");
			}
			topView = !topView;
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


	}
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

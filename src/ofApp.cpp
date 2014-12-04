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
	// draw the vertices in pathLines as a line strip
	pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
//	m_mesh.load("C:\\Users\\Ellison\\Documents\\Models\\bsg.ply");
	m_terrain.load("Terrain.ply");
	//m_terrain.load("Terrain.ply");
	//landImg.loadImage("C:\\Users\\Ellison\\Pictures\\quebec.jpg");  
//	shader.load("shader.vert", "shader.frag");
//	model_shader.load("modelshader.vert", "modelshader.frag");
//	terrain_shader.load("vt.vertexshader", "frag.fragmentshader"/*,"geo.geometryshader" */);
	terrain_shader.load("shader.vert", "shader.frag", "geo.geometryshader");
//	terrain_shader.load("shader.vert", "shader.frag");

	genTerrain();
	current.x = 0;
	current.z = 0;
	current.y =  terrain.maxHeight + defaultHeight;
	target = ofVec3f(0,0,0);

	myImage.loadImage("C:\\Users\\Ellison\\Pictures\\saj.jpg");  
	myTexture = myImage.getTextureReference();  

   	ofSetVerticalSync(true);
	ofEnableDepthTest();
	easyCam.setDistance(100);

    terrain.reset();
    
//    m_terrain = terrain.mesh;
	//terrain.diamondSquareIterationByIdx();
    //diamondSquare(m_terrain, landImg);
}

void ofApp::genTerrain()
{
	for (int i=0; i< 6; i++)
	{
		terrain.diamondSquareIterationByIdx();
	}
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
	previous = current;
	
	
	// generate a noisy 3d position over time 
	
	float t = (2 + ofGetElapsedTimef()) * .1;
	/*
	current.x = ofSignedNoise(t, 0, 0);
	current.y = ofSignedNoise(0, t, 0);
	current.z = ofSignedNoise(0, 0, 5);
	current *= 550*scale; // scale from -1,+1 range to -400,+400
	*/
	
	
	/*
	current.x+=t;
	current *= 550*scale; // scale from -1,+1 range to -400,+400
	*/

	if (abs(current.x) > 550*scale) current.x = 0;

	// add the current position to the pathVertices deque
	pathVertices.push_back(current);
	// if we have too many vertices in the deque, get rid of the oldest ones
	while(pathVertices.size() > 200) {
		pathVertices.pop_front();
	}
	// clear the pathLines ofMesh from any old vertices
	pathLines.clear();
	// add all the vertices from pathVertices
	for(unsigned int i = 0; i < pathVertices.size(); i++) {
		pathLines.addVertex(pathVertices[i]);
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	//glEnable(GL_CULL_FACE);

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

	ofBackgroundGradient(cyan * 1.0, cyan * .4);
	ofNoFill();

	//camera1.setVFlip(true);
	camera1.setPosition((ofVec3f(current.x, current.y, current.z)));
	camera1.lookAt(ofVec3f(current.x, current.y+target.y, current.z+(zoff)));

	camera1.begin();
	//ofTranslate(current.x, current.y, current.z);
	//glPolygonMode(GL_FRONT, GL_FILL);
	myTexture.unbind();  
	
    light1.enable();
    //            light2.enable();
    //            light3.enable();
    light1.setDirectional();
    light1.lookAt(ofVec3f(0, 0, 0));
    light1.setGlobalPosition(ofVec3f(0, 0, 10));

	terrain_shader.begin();
    terrain_shader.setUniform1f("maxHeight", terrain.maxHeight);
    terrain_shader.setUniform1f("minHeight", terrain.minHeight);
    terrain_shader.setUniform1f("scale", scale);
    terrain_shader.setUniform1i("tess", tess);
	ofScale(scale,1.0f,scale);
	ofRotateX(90);
	ofTranslate(0,0,0);
	ofRotateY(180);
	//terrain.draw();
	//m_terrain.draw();
	m_terrain.drawWireframe();
	terrain_shader.end();
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
//    light2.disable();
//    light3.disable();
	camera1.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	float posInc = 10.0f;
	switch (key)
	{
		case OF_KEY_RIGHT:
			current.x+=posInc;
			fprintf(stderr, "Right key pressed. %2.4f\n", current.x);
			break;
		case OF_KEY_LEFT:
			current.x-=posInc;
			fprintf(stderr, "Left key pressed. %2.4f\n", current.x);
			break;
		case OF_KEY_UP:
			current.y+=posInc;
			fprintf(stderr, "Up key pressed. %2.4f\n", current.y);
			break;
		case OF_KEY_DOWN:
			current.y-=posInc;
			fprintf(stderr, "Down key pressed. %2.4f\n", current.y);
			break;
		case OF_KEY_PAGE_UP:
			current.z-=posInc;
			fprintf(stderr, "PageUp key pressed. %2.4f\n", current.z);
			break;
		case OF_KEY_PAGE_DOWN:
			current.z+=posInc;
			fprintf(stderr, "PageDown key pressed. %2.4f\n", current.z);
			break;
		case 'w':
			terrain.setMode();
			fprintf(stderr, "Wireframe\n");
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
			fprintf(stderr, "Terrain reset\n");
			break;
		case 's':
			drawShip = !drawShip;
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

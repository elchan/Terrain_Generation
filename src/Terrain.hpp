//
//  ofTerrain.h
//  quaternionArcballExample
//
//  Created by Alexandre Pretyman on 10/18/14.
//
//

#ifndef __quaternionArcballExample__ofTerrain__
#define __quaternionArcballExample__ofTerrain__
#include "NormalGeneration.hpp"
#include <iostream>
#include "ofMain.h"
#include <cstdlib>
#include <limits>
//#include <glm/glm.hpp>
#include <cmath>

struct Terrain {
    
    typedef std::vector<ofVec3f> VertexVector;
    typedef VertexVector::size_type IndexType;
    
    static int const MAX_ITERATIONS = 8;
	bool  wireframe;
    
	 bool indicesEnabled;
    unsigned int iterations;
    float const unit;
    float const kModifier;
//    float minHeight = std::numeric_limits<float>::max();
//    float maxHeight = std::numeric_limits<float>::min();
    float minHeight;
    float maxHeight;


    Terrain() : indicesEnabled(true), unit(550.f), kModifier(0.0002) {
        
        reset();
		wireframe = false;

    }
  
  void draw(bool wireFrame) {
    if (wireFrame) {
      mesh.drawWireframe();
    }
    else {
      mesh.draw();
    }
  }
    
	void setMode()
	{
		wireframe = !wireframe;
	}

    void reset() {
        iterations = 0;
        mesh.clearVertices();
        mesh.clearIndices();
        
        mesh.addVertex(ofPoint(-unit/2, -unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(-unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, unit/2, 0 /* unit*/));
        mesh.addVertex(ofPoint(unit/2, -unit/2, 0 /* unit*/));
        
        
        
        mesh.addIndex(0);
        mesh.addIndex(1);
        mesh.addIndex(2);
        mesh.addIndex(2);
        mesh.addIndex(3);
        mesh.addIndex(0);
        mesh.enableIndices();

        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void draw() {
        
		vbo.disableNormals();
		mesh.enableNormals();
        if (indicesEnabled) {
			if (wireframe)
				vbo.drawElements(GL_POINTS, vbo.getNumIndices());
			else
				vbo.drawElements(GL_TRIANGLES, vbo.getNumIndices());
        }
        else {
		//fprintf(stderr, "Using normals(draw): %i \n",vbo.getUsingNormals());
            vbo.draw(GL_QUADS, 0, vbo.getNumVertices());
        }
    }
    
  
    
    void diamondSquareIterationByIdx() {
        if (iterations >= MAX_ITERATIONS)
            return;
        indicesEnabled = true;
        auto & previousItVertices = mesh.getVertices();
        auto previousItIndices = mesh.getIndices();
        mesh.clearIndices();
//        mesh.clear();
        iterations++;
        auto previousItIdx = previousItIndices.begin();
        while (previousItIdx != previousItIndices.end()) {
            
            IndexType bottomLeftIdx = *previousItIdx++;
            IndexType topLeftIdx = *previousItIdx++;
            IndexType topRightIdx = *previousItIdx++;
                                    previousItIdx++; // skip top right
            IndexType bottomRightIdx = *previousItIdx++;
                                    previousItIdx++; // skip bottom left
            
            ofVec3f bottomLeft = getVertex(bottomLeftIdx);
            ofVec3f topLeft = getVertex(topLeftIdx);
            ofVec3f topRight = getVertex(topRightIdx);
            ofVec3f bottomRight = getVertex(bottomRightIdx);
            
            
            IndexType middlePointIdx = addVertex(((bottomLeft + bottomRight + topRight + topLeft)/4.f));
            
            disturbMidpoint(getVertex(middlePointIdx), bottomLeft, topLeft, topRight, bottomRight);
            
            IndexType leftMidIdx = addVertex((topLeft + bottomLeft)/2.f);
            IndexType botMidIdx = addVertex((bottomRight + bottomLeft)/2.f);
            IndexType topMidIdx = addVertex((topRight + topLeft)/2.f);
            IndexType rightMidIdx = addVertex((bottomRight + topRight)/2.f);
            
            addQuad(bottomLeftIdx, leftMidIdx, middlePointIdx, botMidIdx);
            addQuad(leftMidIdx, topLeftIdx, topMidIdx, middlePointIdx);
            addQuad(middlePointIdx, topMidIdx, topRightIdx, rightMidIdx);
            addQuad(botMidIdx, middlePointIdx, rightMidIdx, bottomRightIdx);
            
        }

        std::cout << "Diamond Square iterations: " << iterations << " resulted in " << mesh.getNumVertices() << " vertices, " << mesh.getNumIndices() / 4 << " quads" << std::endl;
      generateVertexNormal(mesh);
      updateTexCoordinates();
        std::cout << "MinHeight: " << minHeight << " MaxHeight: " << maxHeight << std::endl;
        
    }
    
    void save() {
        mesh.save("./NewTerrain.ply");
    }
    
    void load() {
        mesh.load("Terrain.ply");
        minHeight = 0;
        maxHeight = 0;
        for (auto & vec : mesh.getVertices()) {
            if (vec.z > maxHeight) {
                maxHeight = vec.z;
            }
            else if (vec.z < minHeight) {
                minHeight = vec.z;
            }
        }
        vbo.clear(); // clear what is in the GPU
        vbo.setMesh(mesh, GL_STATIC_DRAW);
    }
    
    void iterateTo(unsigned int iteration) {
        if (iteration > iterations) {
            diamondSquareIterationByIdx();
        }
    }
  
  void updateTexCoordinates() {
    mesh.clearTexCoords();
    auto bottomLeft = ofVec2f(-unit/2.f, -unit/2.f);
    for (auto & vertex : mesh.getVertices()) {
		mesh.addTexCoord(ofVec2f(1024.f * (vertex.x-bottomLeft.x)/unit, 1024.f * (vertex.y-bottomLeft.y)/unit));
    }
    
  }
    
    float randomValue() const {
        return std::rand() % 2 == 0 ? unit * kModifier : - unit * kModifier;
//        return iterations % 2 == 0 ? unit * kModifier  : - unit * kModifier;
    }
   
    std::vector<ofVec4f> colors;
    
	/*
   v1        v2
    +---------+
    |         | 
    |         |
    +---------+
    v3        v4
	*/
    ofVec3f crossProd(ofVec3f center, ofVec3f v1, ofVec3f v2)
	{
		return (v1-center).getCrossed((v2-center));
	}
    ofMesh mesh;
    ofVbo vbo;
private:
    /**
     * Add a quad to the mesh with clockwise front face vertex order
     */
    void addQuad(ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) {
		mesh.enableNormals();
		vbo.enableNormals();
		ofVec3f v1 = topLeft;
		ofVec3f v2 = topRight;
		ofVec3f v3 = bottomRight;
		ofVec3f v4 = bottomLeft;
		ofVec3f v1N, v2N, v3N, v4N;
		v1N =  crossProd(v1,v2,v3);
		v3N =  crossProd(v3,v4,v1);
		v2N =  crossProd(v2,v1,v4);
		v4N =  crossProd(v4,v3,v2);

        mesh.addVertex(v4);
		mesh.addNormal(v4N);
        mesh.addVertex(v1);
		mesh.addNormal(v1N);
        mesh.addVertex(v2);
		mesh.addNormal(v2N);
        mesh.addVertex(v3);
		mesh.addNormal(v3N);
    }
    
    /**
     * Add a quad, using indices, with clockwise front face vertex order
     */
    void addQuad(IndexType const bottomLeft, IndexType const topLeft, IndexType const topRight, IndexType const bottomRight) {
        mesh.addIndex(bottomLeft);
        mesh.addIndex(topLeft);
        mesh.addIndex(topRight);
        mesh.addIndex(topRight);
        mesh.addIndex(bottomRight);

		mesh.addIndex(bottomLeft);
    }
    
    IndexType addVertex(ofVec3f const & vertex) {
        mesh.getVertices().push_back(vertex);
        return mesh.getVertices().size()-1;
    }
    
    ofVec3f & getVertex(IndexType const idx) {
        return mesh.getVertices()[idx];
    }
    
    
    void applyElevationFunction(ofVec3f & point) {
//        point.z = (std::sin(point.x * ( 2 * PI / unit)) +  std::sin(point.y * (PI / unit)))  * 50;
        auto x = point.x * ( 2 * PI / unit);
        auto y = point.y * ( 2 * PI / unit);
        point.z = std::sqrt(std::sin(x * x +  y * y)) * 50;
    }
    
    void disturbMidpoint(ofVec3f & midPoint, ofVec3f const & bottomLeft, ofVec3f const & topLeft, ofVec3f const & topRight, ofVec3f const & bottomRight) {
      //if (std::rand() % 2 == 0) {
      //  return;
      //}
        midPoint[2] += (topLeft.distance(bottomRight) + topRight.distance(bottomLeft)) * randomValue();
        
        if (midPoint[2] < minHeight) {
            minHeight = midPoint[2];
        }
        if (midPoint[2] > maxHeight) {
            maxHeight = midPoint[2];
        }

    }
};

#endif /* defined(__quaternionArcballExample__ofTerrain__) */

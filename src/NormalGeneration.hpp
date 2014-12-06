#pragma once
#include "ofMain.h"
#include <cassert>
struct Triangle {
    ofMesh * mesh;
    ofIndexType indices[3];
    
    Triangle(ofMesh * mesh, ofIndexType idx0, ofIndexType idx1, ofIndexType idx2)
    : mesh(mesh) {
        indices[0] = idx0;
        indices[1] = idx1;
        indices[2] = idx2;
    }
    
    ofVec3f get(ofIndexType idx) const {
        return mesh->getVertices()[indices[idx]];
    }
    
    ofVec3f normal() const {
      // clock wise order 
        return (get(2) - get(0)).cross((get(1) - get(0))).normalize();
    }
    
    bool has(ofIndexType idx) const {
        return indices[0] == idx || indices[1] == idx || indices[2] == idx;
    }
    
    bool has(ofIndexType idx0, ofIndexType idx1) const { // has edge
        return has(idx0) && has(idx1);
    }
};
inline void generateVertexNormal(ofMesh & mesh) {
    assert(mesh.getNumIndices() % 3 == 0);
    mesh.getNormals().clear(); // we clear here to make sure our first normal pushed back
    mesh.getNormals().reserve(mesh.getNumIndices());
    // will be the same
    std::vector<Triangle> neighbors;
    neighbors.reserve(10);
    for (ofIndexType i = 0; i < mesh.getNumVertices(); i++) {
        neighbors.clear();
        for (ofIndexType j = 0; j < mesh.getNumIndices(); j+=3) {
            Triangle triangle(&mesh, mesh.getIndex(j), mesh.getIndex(j+1), mesh.getIndex(j+2));
            if (triangle.has(i)) {
                neighbors.push_back(triangle);
            }
        }
        ofVec3f averageNormal;
        for (auto face : neighbors) {
            averageNormal = averageNormal + face.normal();
        }
        averageNormal = (averageNormal / neighbors.size()).normalize();
        mesh.addNormal(averageNormal); // adds a Normal for vertex i
    }
}
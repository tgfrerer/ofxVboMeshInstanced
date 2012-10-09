//
//  ofxInstancedVboMesh.h
//  sketchManySpikes
//
//  Created by tim on 03/10/2012.
//
//

#pragma once

#include "ofMesh.h"
#include "ofVbo.h"

class ofxVboMeshInstanced: public ofMesh {
public:
	using ofMesh::draw;
	ofxVboMeshInstanced();
	ofxVboMeshInstanced(const ofMesh & mom);
	void setUsage(int usage);

	void setPrimCount(int primCount_){primCount = primCount_;};
	int getPrimCount(){return primCount;};
	
protected:
	void draw(ofPolyRenderMode drawMode);
	
private:
	void drawElementsInstanced(GLint mode, int num, int primCount);
	void drawArraysInstanced(GLint mode, int num, int primCount);
	
	ofVbo vbo;
	int usage;
	int primCount;
	int vboNumVerts, vboNumIndices, vboNumNormals, vboNumTexCoords, vboNumColors;
};

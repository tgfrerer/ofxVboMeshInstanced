//
//  ofxInstancedVboMesh.cpp
//  sketchManySpikes
//
//  Created by tim on 03/10/2012.
//
//

#include "ofxVboMeshInstanced.h"

ofxVboMeshInstanced::ofxVboMeshInstanced(){
	usage= GL_STATIC_DRAW;
	primCount = 1;
}

// ----------------------------------------------------------------------

ofxVboMeshInstanced::ofxVboMeshInstanced(const ofMesh & mom)
:ofMesh(mom)
,primCount(1)
{
	usage= GL_STATIC_DRAW;
}

// ----------------------------------------------------------------------

void ofxVboMeshInstanced::setUsage(int _usage){
	usage = _usage;
}

// ----------------------------------------------------------------------

void ofxVboMeshInstanced::drawElementsInstanced(GLint mode, int num, int primCount){
	if(vbo.getIsAllocated()){
		vbo.bind();
		if(vbo.getUsingIndices()){
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo.getIndexId());
#ifdef TARGET_OPENGLES
			glDrawElementsInstanced(mode, num, GL_UNSIGNED_SHORT, NULL, primCount);
#else
			glDrawElementsInstanced(mode, num, GL_UNSIGNED_INT, NULL, primCount);
#endif
		}
		vbo.unbind();
	}
}

// ----------------------------------------------------------------------

void ofxVboMeshInstanced::drawArraysInstanced(GLint mode, int num, int primCount){
	if(vbo.getIsAllocated()) {
		vbo.bind();
		glDrawArraysInstanced(mode, 0, getNumVertices(),primCount);
		vbo.unbind();
	} else{
		ofLogWarning() << "cannot draw vbo instanced since vbo not allocated.";
	}

}

// ----------------------------------------------------------------------

void ofxVboMeshInstanced::draw(ofPolyRenderMode drawMode){
	if(!vbo.getIsAllocated()){
		if(getNumVertices()){
			vbo.setVertexData(getVerticesPointer(),getNumVertices(),usage);
		}
		if(getNumColors()){
			vbo.setColorData(getColorsPointer(),getNumColors(),usage);
		}
		if(getNumNormals()){
			vbo.setNormalData(getNormalsPointer(),getNumNormals(),usage);
		}
		if(getNumTexCoords()){
			vbo.setTexCoordData(getTexCoordsPointer(),getNumTexCoords(),usage);
		}
		if(getNumIndices()){
			vbo.setIndexData(getIndexPointer(),getNumIndices(),usage);
		}
		vboNumIndices = getNumIndices();
		vboNumVerts = getNumVertices();
		vboNumColors = getNumColors();
		vboNumTexCoords = getNumTexCoords();
		vboNumNormals = getNumNormals();
	}
	
	if(haveVertsChanged()){
		if(vboNumVerts<getNumVertices()){
			vbo.setVertexData(getVerticesPointer(),getNumVertices(),usage);
			vboNumVerts = getNumVertices();
		}else{
			vbo.updateVertexData(getVerticesPointer(),getNumVertices());
		}
	}
	if(haveColorsChanged()){
		if(vboNumColors<getNumColors()){
			vbo.setColorData(getColorsPointer(),getNumColors(),usage);
			vboNumColors = getNumColors();
		}else{
			vbo.updateColorData(getColorsPointer(),getNumColors());
			
		}
	}
	if(haveNormalsChanged()){
		if(vboNumNormals<getNumNormals()){
			vbo.updateNormalData(getNormalsPointer(),getNumNormals());
			vboNumNormals = getNumNormals();
		}else{
			vbo.setNormalData(getNormalsPointer(),getNumNormals(),usage);
		}
	}
	if(haveTexCoordsChanged()){
		if(vboNumTexCoords<getNumTexCoords()){
			vbo.setTexCoordData(getTexCoordsPointer(),getNumTexCoords(),usage);
			vboNumTexCoords = getNumTexCoords();
		}else{
			vbo.updateTexCoordData(getTexCoordsPointer(),getNumTexCoords());
		}
	}
	if(haveIndicesChanged()){
		if(vboNumIndices<getNumIndices()){
			vbo.setIndexData(getIndexPointer(),getNumIndices(),usage);
			vboNumIndices = getNumIndices();
		}else{
			vbo.updateIndexData(getIndexPointer(),getNumIndices());
		}
	}
	
	
	GLuint mode = ofGetGLPrimitiveMode(getMode());
	
	
	if (primCount <=1){
#ifndef TARGET_OPENGLES
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(drawMode));
		if(getNumIndices() && drawMode!=OF_MESH_POINTS){
			vbo.drawElements(mode,getNumIndices());
		}else{
			vbo.draw(mode,0,getNumVertices());
		}
		glPopAttrib();
#else
		if(drawMode == OF_MESH_POINTS){
			vbo.draw(GL_POINTS,0,getNumVertices());
		}else if(drawMode == OF_MESH_WIREFRAME){
			if(getNumIndices()){
				vbo.drawElements(GL_LINES,getNumIndices());
			}else{
				vbo.draw(GL_LINES,0,getNumVertices());
			}
		}else{
			if(getNumIndices() && drawMode!=OF_MESH_POINTS){
				vbo.drawElements(mode,getNumIndices());
			}else{
				vbo.draw(mode,0,getNumVertices());
			}
		}
#endif
	} else {
		
		// ----------| invariant: primCount > 1
		
		// this is where we get funky. we want more than one instance to be drawn.
		
#ifndef TARGET_OPENGLES
		glPushAttrib(GL_POLYGON_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, ofGetGLPolyMode(drawMode));
		if(getNumIndices() && drawMode!=OF_MESH_POINTS){
			drawElementsInstanced(mode,getNumIndices(),primCount);
		}else{
			drawArraysInstanced(mode, getNumVertices(),primCount);
		}
		glPopAttrib();
#else
		// TODO: Doublecheck this. 
		if(drawMode == OF_MESH_POINTS){
			vbo.draw(GL_POINTS,0,getNumVertices());
		}else if(drawMode == OF_MESH_WIREFRAME){
			if(getNumIndices()){
				drawElementsInstanced(GL_LINES,getNumIndices(), primCount);
			}else{
				drawArraysInstanced(GL_LINES, getNumVertices(),primCount);
			}
		}else{
			if(getNumIndices() && drawMode!=OF_MESH_POINTS){
				drawElementsInstanced(mode,getNumIndices(),primCount);
			}else{
				drawArraysInstanced(mode,0,getNumVertices());
			}
		}
#endif
	}
	
}

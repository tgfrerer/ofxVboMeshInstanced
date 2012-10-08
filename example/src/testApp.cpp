#include "testApp.h"


/*    _____    ___
 *   /    /   /  /     ofxVboMeshInstancedExample
 *  /  __/ * /  /__    (c) ponies & light, 2012.
 * /__/     /_____/    poniesandlight.co.uk
 *
 * Created by Timothy Gfrerer on 03/10/2012.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */


#define NUMBOXES 128

//--------------------------------------------------------------
void testApp::setup(){
	
//	ofSetWindowPosition(1900, -100);
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofSetColor(255);

	
	mPnlMain.setup("MAIN SETTINGS", "settings.xml", 20, 20);
	mPnlMain.add(mPrmFloat["zNear"].set("zNear",0,0,100));
	mPnlMain.add(mPrmFloat["zFar"].set("zFar",0,0,5000.0));

	mPnlMain.add(mPrmFloat["k"].set("k", 0.01, 0.0, 0.3));
	mPnlMain.add(mPrmFloat["d"].set("d", 0.967, 0.5, 1.0));
	mPnlMain.add(mPrmBool["shouldRenderFBOs"].set("shouldRenderFBOs", true));
	mPnlMain.add(mPrmBool["shouldRenderNormals"].set("shouldRenderNormals", true));
	mPnlMain.add(mPrmBool["shouldUseFlatShading"].set("shouldUseFlatShading", true));

	mPnlMain.loadFromFile("settings.xml");
	
	
	mFboForce[0].allocate(NUMBOXES, NUMBOXES);
	mFboForce[1].allocate(NUMBOXES, NUMBOXES);
	
	mFboLength[0].allocate(NUMBOXES, NUMBOXES);
	mFboLength[1].allocate(NUMBOXES, NUMBOXES);


	ofImage noiseImg;
	noiseImg.loadImage("128logo.png");
	
	noiseImg.getTextureReference().setCompression(OF_COMPRESS_NONE);
	
	mFboForce[0].getTextureReference().setCompression(OF_COMPRESS_NONE);
	mFboForce[1].getTextureReference().setCompression(OF_COMPRESS_NONE);

	mFboLength[0].getTextureReference().setCompression(OF_COMPRESS_NONE);
	mFboLength[1].getTextureReference().setCompression(OF_COMPRESS_NONE);

	
	mFboForce[0].begin();
	noiseImg.draw(0, 0);
	mFboForce[0].end();

	mFboForce[1].begin();
	ofClear(0, 255, 0);
	mFboForce[1].end();

	mFboLength[0].begin();
	ofClear(128);
	mFboLength[0].end();

	mFboLength[1].begin();
	ofClear(128);
	mFboLength[1].end();

	mPingPong = false;
	isShaderDirty = true;
	
	
	// create cube mesh
	
	ofVec3f vertices[] = {
		
		ofVec3f(-1, -1,  1),		// front square vertices
		ofVec3f( 1, -1,  1),
		ofVec3f( 1,  1,  1),
		ofVec3f(-1,  1,  1),
		
		ofVec3f(-1, -1, -1),		// back square vertices
		ofVec3f( 1, -1, -1),
		ofVec3f( 1,  1, -1),
		ofVec3f(-1,  1, -1),
		
	};
	
	ofIndexType indices[] = {
		// -- winding is counter-clockwise (facing camera)
		0,1,2,		// pos z
		0,2,3,
		1,5,6,		// pos x
		1,6,2,
		2,6,7,		// pos y
		2,7,3,
		
		// -- winding is clockwise (facing away from camera)
		3,4,0,		// neg x
		3,7,4,
		4,5,1,		// neg y
		4,1,0,
		5,7,6,		// neg z
		5,4,7,
	};
	
	ofVec3f normals[] = {
		ofVec3f( 0,  0,  1),
		ofVec3f( 1,  0,  0),
		ofVec3f( 0,  1,  0),
		ofVec3f(-1,  0,  0),
		ofVec3f( 0, -1,  0),
		ofVec3f( 0,  0, -1),
		ofVec3f(1,0,0), // can be anything, will not be used
		ofVec3f(1,0,0), //  -- " --
	};
	
	mMshCube.addVertices(vertices, 8);
	mMshCube.addNormals(normals,8);
	mMshCube.addIndices(indices, 3*2*6);
	mMshCube.setPrimCount(NUMBOXES*NUMBOXES);
	
	mLigDirectional.setup();
	mLigDirectional.setDirectional();
	mLigDirectional.setAmbientColor(ofColor::fromHsb(0, 0, 200));
	mLigDirectional.setDiffuseColor(ofColor::fromHsb(120, 120, 128));
	mLigDirectional.setSpecularColor(ofColor(255,255,255));

	
	mMatMainMaterial.setDiffuseColor(ofColor(0,0,0));
	mMatMainMaterial.setSpecularColor(ofColor(200,200,200));
	mMatMainMaterial.setShininess(25.0f);

	
	mCamMain.setupPerspective(false);
}

//--------------------------------------------------------------

void testApp::update(){
	if (isShaderDirty){
		
		GLuint err = glGetError();	// we need this to clear out the error buffer.
		
		if (mShdForce != NULL ) delete mShdForce;
		mShdForce = new ofShader();
		mShdForce->load("shaders/force");
		err = glGetError();	// we need this to clear out the error buffer.
		ofLogNotice() << "Loaded Force Shader: " << err;
		
		if (mShdLength != NULL ) delete mShdLength;
		mShdLength = new ofShader();
		mShdLength->load("shaders/length");
		err = glGetError();	// we need this to clear out the error buffer.
		ofLogNotice() << "Loaded Length Shader: " << err;

		if (mShdInstanced != NULL ) delete mShdInstanced;
		mShdInstanced = new ofShader();
		mShdInstanced->load("shaders/instanced");
		err = glGetError();	// we need this to clear out the error buffer.
		ofLogNotice() << "Loaded instanced Shader: " << err;

		isShaderDirty = false;
	}
	
	mCamMain.setNearClip(mPrmFloat["zNear"]);
	mCamMain.setFarClip(mPrmFloat["zFar"]);

	
	mPingPong ^= true;
}

//--------------------------------------------------------------

void testApp::draw(){
	
	// update strength to fbo
	ofSetColor(255);
	// update force on fbo
	mFboForce[mPingPong].begin();
	mShdForce->begin();
	mShdForce->setUniform1f("k",mPrmFloat["k"]);
	mShdForce->setUniform1f("d",mPrmFloat["d"]);
	mShdForce->setUniformTexture("lengthTex", mFboLength[mPingPong], 1);
	mFboForce[mPingPong ^ true].draw(0, 0);
	mShdForce->end();
	mFboForce[mPingPong].end();

	// update length on fbo
	mFboLength[mPingPong].begin();
	mShdLength->begin();
	mShdLength->setUniformTexture("forceTex", mFboForce[mPingPong], 1);
	mFboLength[mPingPong ^ true].draw(0, 0);
	mShdLength->end();
	mFboLength[mPingPong].end();

	// render instanced box, with shader z-modification according to length.

	ofBackgroundGradient(ofColor::fromHsb(0, 0, 120), ofColor::fromHsb(0, 0, 0));
	
	mCamMain.begin();

	ofEnableLighting();
	
	mLigDirectional.setGlobalPosition(1000, 1000, 1000);
	mLigDirectional.lookAt(ofVec3f(0,0,0));
	
	ofEnableSeparateSpecularLight();

	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	mShdInstanced->begin();

	if (mPrmBool["shouldRenderNormals"]){
		mShdInstanced->setUniform1f("shouldRenderNormals", 1.0);
	} else {
		mShdInstanced->setUniform1f("shouldRenderNormals", 0.0);
	}

	
	if (mPrmBool["shouldUseFlatShading"]){
		mShdInstanced->setUniform1f("shouldUseFlatShading", 1.0);
		glShadeModel(GL_FLAT);
		glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);		// OpenGL default is GL_LAST_VERTEX_CONVENTION
	} else {
		mShdInstanced->setUniform1f("shouldUseFlatShading", 0.0);
		glShadeModel(GL_SMOOTH);
		glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	}

	
	mLigDirectional.enable();
	mMatMainMaterial.begin();
	ofSetColor(128, 0, 0);
	mMshCube.draw();
	ofSetColor(255);
	mMatMainMaterial.end();
	mLigDirectional.disable();
	
	mShdInstanced->end();
	
	glProvokingVertex(GL_LAST_VERTEX_CONVENTION);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);

	ofDisableLighting();
	
	mCamMain.end();
	
	if (mPrmBool["shouldRenderFBOs"]){
		mFboForce[mPingPong].draw(200, 10, 256, 256);
		mFboLength[mPingPong].draw(200, 10+10+256, 256, 256);
		
		mFboForce[mPingPong^true].draw(200 + 10 + 256, 10, 256, 256);
		mFboLength[mPingPong^true].draw(200 + 10 + 256, 10+10+256, 256, 256);
		
	}
	
	
	mPnlMain.draw();
	
	ofDrawBitmapString(ofToString(ofGetFrameRate()), ofGetWidth()-100, 20);
	
}

//--------------------------------------------------------------

void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------

void testApp::keyReleased(int key){
	
	switch (key) {
		case ' ':
		{
			isShaderDirty = true;

			ofImage noiseImg;
			noiseImg.loadImage("128logo.png");
			
			mFboLength[mPingPong ^ true].begin();
			noiseImg.draw(0, 0);
			mFboLength[mPingPong ^ true].end();

			mFboLength[mPingPong].begin();
			ofClear(128);
			mFboLength[mPingPong].end();
			
			mFboForce[mPingPong ^ true].begin();
			ofClear(128);
			mFboForce[mPingPong ^ true].end();

			mFboForce[mPingPong].begin();
			ofClear(128);
			mFboForce[mPingPong].end();

		}
			break;
			
			case 'f':
			ofToggleFullscreen();
			break;
			
		default:
			break;
	}
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}
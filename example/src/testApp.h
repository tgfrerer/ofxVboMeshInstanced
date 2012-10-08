#pragma once

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

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxVboMeshInstanced.h"

class testApp : public ofBaseApp{

	bool	isShaderDirty;
	
	ofFbo	mFboForce[2];
	ofFbo	mFboLength[2];

	ofShader* mShdForce;
	ofShader* mShdLength;
	ofShader* mShdInstanced;

	bool	mPingPong;

	ofxVboMeshInstanced mMshCube;
	
	ofEasyCam	mCamMain;
	
	ofLight	mLigDirectional;
	ofMaterial mMatMainMaterial;
	
	ofxPanel	mPnlMain;
	map<string, ofxParameter<float> > mPrmFloat;
	map<string, ofxParameter<bool> > mPrmBool;
	
	public:
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
};

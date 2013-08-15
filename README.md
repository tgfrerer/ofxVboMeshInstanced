<pre>
     _____    ___     
    /    /   /  /     ofxVboMeshInstanced
   /  __/ * /  /__    (c) ponies & light, 2012. 
  /__/     /_____/    poniesandlight.co.uk

  ofxVboMeshInstanced
  Created by Tim Gfrerer 2012.
  
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
</pre>


# ofxVboMeshInstanced

## Update

This addon is deprecated with openFrameworks 0.8.0

As of openFrameworks 0.8.0, the functionality of ofxVboMeshInstanced has been merged 
into openFrameworks core. I've added an example project, ofDrawVboMeshInstancedExample,
to the official openFrameworks example directory. Find it here:

https://github.com/openframeworks/openFrameworks/tree/master/examples/gl/vboMeshDrawInstancedExample

## Draw instanced VBO meshes with openFrameworks.

This addon helps drawing instanced openGL meshes with openFrameworks, and
the enclosed example demonstrates the technique with a GPU accelerated 
reaction-diffusion system. 

# Example project

Use the openFrameworks Project Generator to create project files for the example.

Don't forget to select:

* ofxVboMeshInstanced
* ofxGui
* ofxXmlSettings

from addons before generating the project file.

## Reference Render

![reference render](http://poniesandlight.co.uk/img/boxes_teaser.jpg)

See also https://vimeo.com/50841645 for a demo.

## Dependencies:

<pre>
	* openFrameworks >= 0071
	* ofxGui (core addon)
	* ofxXmlSettings (core addon)
</pre>
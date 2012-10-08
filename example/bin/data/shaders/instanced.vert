#extension GL_EXT_gpu_shader4 : require

// phong shader code pilfered from http://www.ozone3d.net/tutorials/glsl_lighting_phong.php

// we need these for Phong shading
uniform sampler2DRect rnm;

varying vec3 normal, screenSpaceNormal, lightDir, eyeVec;

// this is where the magic happens. notice 'flat' before varying
// and the compiler instruction for the extension at the top of this file.

flat varying vec3  flatNormal;
flat varying vec3  screenSpaceflatNormal;

// these switches set through oF

uniform float shouldRenderNormals;
uniform float shouldUseFlatShading;

varying vec2 texCoord;

int		iCOUNT = 128 ;
float	fCOUNT = float(iCOUNT);

void main( void )
{
	
	float instanceX = float(gl_InstanceID%(iCOUNT) - iCOUNT/2);
	float instanceY = float(gl_InstanceID/(iCOUNT) - iCOUNT/2);
	
	vec3 inNormal = gl_Normal ;
	vec4 inVertex = gl_Vertex + vec4(4.5,0.0,0.0,0.0) * instanceX
	+ vec4(0.0,4.5,0.0,0.0) * instanceY
	+	vec4(0.0,0.0,5.0,0.0) * sin(instanceX/(fCOUNT * 0.5) * 3.141516);
	
	vec4 texSample = texture2DRect(rnm,vec2((instanceX +fCOUNT/2.0) * 1.0 ,(instanceY +fCOUNT/2.0) * 1.0));
	inVertex.z += (texSample.r-0.5) * 200.0;
	
	screenSpaceNormal = (gl_ModelViewMatrix * vec4(inNormal,1.0)).xyz;
	screenSpaceflatNormal = (gl_ModelViewMatrix * vec4(inNormal,1.0)).xyz;
	
	normal = gl_NormalMatrix * inNormal;
	flatNormal = gl_NormalMatrix * inNormal;
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * inVertex);
	
	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;
	
	texCoord = gl_MultiTexCoord0.xy;
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * inVertex;
}
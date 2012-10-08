#extension GL_EXT_gpu_shader4 : require

uniform sampler2DRect forceTex;
uniform sampler2DRect lengthTex;

varying vec2 texCoord;
uniform float k;
uniform float d;

void main( void )
{
	texCoord = gl_MultiTexCoord0.xy;
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
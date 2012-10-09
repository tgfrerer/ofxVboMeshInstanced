#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : require

uniform sampler2DRect lengthTex;
uniform sampler2DRect forceTex;

varying vec2 texCoord;


void main( void )
{
	
	vec4 forceSample = texture2DRect(forceTex,  texCoord);
	vec4 lengthSample = texture2DRect(lengthTex, texCoord);
	
	// map currentForce to -1..1
	float currentForce  = (forceSample.r - 0.5) * 2.0;
	float currentLength = (lengthSample.r - 0.5) * 2.0;
	
	currentLength = currentLength + currentForce;
	
	// map currentForce to 0..1
	
	currentLength = (currentLength * 0.5) + 0.5;
	
	vec4 lengthColor = vec4(currentLength,currentLength,currentLength,1.0);
	
	gl_FragColor = lengthColor;
	
}
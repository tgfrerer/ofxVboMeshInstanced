#extension GL_EXT_gpu_shader4 : require

uniform sampler2DRect forceTex;
uniform sampler2DRect lengthTex;

varying vec2 texCoord;
uniform float k;
uniform float d;


void main( void )
{
	
	vec4 forceSample = vec4(0.0,0.0,0.0,0.0);

	forceSample += texture2DRect(forceTex,  texCoord + vec2( 0.0, 0.0));

	vec4 neighbourSample = vec4(0.0,0.0,0.0,0.0);
	
	neighbourSample += texture2DRect(forceTex,  texCoord + vec2(-1.0, 0.0));
	//		forceSample += texture2DRect(forceTex,  texCoord + vec2(-1.0, 1.0));
	//		forceSample += texture2DRect(forceTex,  texCoord + vec2(-1.0,-1.0));
	
	neighbourSample += texture2DRect(forceTex,  texCoord + vec2( 1.0, 0.0));
//	neighbourSample += texture2DRect(forceTex,  texCoord + vec2( 1.0, 1.0));
	//		forceSample += texture2DRect(forceTex,  texCoord + vec2( 1.0,-1.0));
	
	neighbourSample += texture2DRect(forceTex,  texCoord + vec2( 0.0, 1.0));
	neighbourSample += texture2DRect(forceTex,  texCoord + vec2( 0.0, -1.0));
	
	neighbourSample /= 4.0;

	
	float force = length(neighbourSample);
	
	if (force < 0.37 || force > 0.6){
		forceSample /= neighbourSample;
		forceSample /= 2.0;
	}
	


	
	vec4 lengthSample = texture2DRect(lengthTex, texCoord);
	
	// map currentForce to -1..1
	float currentForce  = (forceSample.r - 0.5) * 2.0;
	float currentLength = (lengthSample.r - 0.5) * 2.0;

	float restLength = 0.0;
	
	currentForce = (currentForce -k * (currentLength - restLength)) * d;
	// currentLength += currentForce;

	// map currentForce to 0..1
	
	currentForce = clamp((currentForce * 0.5) + 0.5, 0.0, 1.0);
	
	vec4 forceColor = vec4(currentForce,currentForce,currentForce,1.0);
	
	gl_FragColor = forceColor;

}
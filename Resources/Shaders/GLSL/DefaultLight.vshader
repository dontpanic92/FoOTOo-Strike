#version 130

uniform mat4 mvMatrix;
uniform mat4 pMatrix;
uniform vec4 vColor;

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;

out vec4 vFragColor;
out vec2 vFragTexCoord;

void main(void) {
    vec3 vLightPosition = vec3(0.0, 1.0, 1.0);
    
    
    vec4 vPosition4 = mvMatrix * vVertex; 
    vec3 vPosition3 = vPosition4.xyz / vPosition4.w;

    vec3 vLightDir = normalize(vLightPosition - vPosition3);

    mat3 mNormalMatrix;
	mNormalMatrix[0] = mvMatrix[0].xyz;
	mNormalMatrix[1] = mvMatrix[1].xyz;
	mNormalMatrix[2] = mvMatrix[2].xyz;
	vec3 vNorm = normalize(mNormalMatrix * vNormal);
	
	float fDot = max(0.0, dot(vNorm, vLightDir));
	vFragColor.rgb = vColor.rgb * fDot;
	vFragColor.a = vColor.a;
	vFragTexCoord = vTexCoord0;
	
	mat4 mvpMatrix;
	mvpMatrix = pMatrix * mvMatrix;
	gl_Position = mvpMatrix * vVertex;
}

#version 130

uniform mat4 mMatrix;
uniform mat4 vMatrix;
uniform mat4 pMatrix;
uniform vec4 vColor;

attribute vec4 vVertex;
attribute vec3 vNormal;
attribute vec2 vTexCoord0;

out vec4 vFragColor;
out vec2 vFragTexCoord;
out vec3 fNormal;
out mat3 NormalMatrix;

void main(void) {
mat4 mvMatrix = vMatrix * mMatrix;
	NormalMatrix[0] = mvMatrix[0].xyz;
	NormalMatrix[1] = mvMatrix[1].xyz;
	NormalMatrix[2] = mvMatrix[2].xyz;

	vec3 vNorm = normalize(NormalMatrix * vNormal);

	NormalMatrix[0] = vMatrix[0].xyz;
	NormalMatrix[1] = vMatrix[1].xyz;
	NormalMatrix[2] = vMatrix[2].xyz;


	vFragColor = vColor;
	vFragTexCoord = vTexCoord0;
	fNormal = vNorm;
	
	mat4 mvpMatrix;
	mvpMatrix = pMatrix * mvMatrix;
	gl_Position = mvpMatrix * vVertex;
}

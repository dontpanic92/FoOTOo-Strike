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
out vec3 lightDirection;

void main(void) {
mat4 mvMatrix = vMatrix * mMatrix;

	fNormal  = normalize(mat3(mvMatrix)* vNormal);

	vFragTexCoord = vTexCoord0;
	vFragColor = vColor;
	gl_Position =  pMatrix * vMatrix * mMatrix * vVertex;
	lightDirection = mat3(vMatrix) * vec3(1.0, 1.0, 1.0);
}

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

out vec3 Normal;

void main(void) {
mat4 mvMatrix = vMatrix * mMatrix;
    mat3 mNormalMatrix;
	mNormalMatrix[0] = mMatrix[0].xyz;
	mNormalMatrix[1] = mMatrix[1].xyz;
	mNormalMatrix[2] = mMatrix[2].xyz;

    vec3 vLightDir = vec3(1.0, 1.0, 1.0);
	vec3 vNorm = normalize(mNormalMatrix * vNormal);
        vec3 vLightDir2 = normalize(mNormalMatrix * vLightDir);
	//Normal = vNormal;

	float fDot = max(0.3, dot(vNorm, vLightDir));
//if(vNorm.y > 0)
	//vFragColor.rgb = vNorm;
//else
	//vFragColor.rgb = vec3(0.0, 0.0, 0.0);
	vFragColor.rgb = vColor.rgb * fDot;// * 0.2;
	vFragColor.a = vColor.a;
	vFragTexCoord = vTexCoord0;
	
	mat4 mvpMatrix;
	mvpMatrix = pMatrix * mvMatrix;
	gl_Position = mvpMatrix * vVertex;
}

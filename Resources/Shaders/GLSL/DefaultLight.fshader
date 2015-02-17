#version 130

in vec4 vFragColor;
in vec2 vFragTexCoord;
in vec3 fNormal;
//in mat3 NormalMatrix;
in vec3 lightDirection;
uniform sampler2D textureUnit0;

void main(void) {
	//vec3 light = vec3(1.0, 1.0, 1.0);
	//vec3 lightDirection = NormalMatrix * light;
	

	float diffuse = max(0.0, dot(fNormal, lightDirection));

	float lightIntensity = diffuse;

	vec3 reflection = normalize(reflect(-lightDirection, fNormal));
	float specular = max(0.0, dot(fNormal, reflection));
	if(diffuse != 0) {
		float fSpec = pow(specular, 128.0); 
		lightIntensity += fSpec;
	}

	vec3 rgb = min(vFragColor.rgb * lightIntensity + vec3(0.2), vec3(1.0));
	gl_FragColor = vec4(rgb, 1.0) * texture2D(textureUnit0, vFragTexCoord);
}
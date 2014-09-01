#version 130

in vec4 vFragColor;
in vec2 vFragTexCoord;
in vec3 Normal;
uniform sampler2D textureUnit0;

void main(void) {
    //float diffuse = max(0.0, dot(Normal, normalize(vec3(1.0, 1.0, 1.0))));
    gl_FragColor = vFragColor * texture2D(textureUnit0, vFragTexCoord);
}
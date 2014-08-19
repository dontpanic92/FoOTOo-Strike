#version 130

in vec4 vFragColor;
in vec2 vFragTexCoord;
uniform sampler2D textureUnit0;

void main(void) {
    gl_FragColor = vFragColor * texture2D(textureUnit0, vFragTexCoord);
}
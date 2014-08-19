uniform mat4 mvMatrix;
uniform mat4 pMatrix;
attribute vec4 vVertex;
void main(void)
{ 
    gl_Position = pMatrix * mvMatrix * vVertex;
}
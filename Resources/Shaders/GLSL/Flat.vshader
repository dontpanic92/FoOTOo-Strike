uniform mat4 mvpMatrix;
attribute vec4 vVertex;
void main(void)
{ 
    gl_Position = mvpMatrix * vVertex;
}
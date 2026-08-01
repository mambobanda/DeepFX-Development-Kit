#version 150 

in vec3 position;


uniform mat4 projMat;
uniform mat4 viewMat;
uniform mat4 modMat;

void main()
{
    gl_BackColor = gl_Color;
    gl_FrontColor = gl_Color;
    gl_Position = projMat * viewMat * modMat * vec4(position, 1.0);
}
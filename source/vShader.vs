#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 transform;
void main()
{
   gl_Position = transform *vec4(aPos.x*0.003, aPos.y*0.003, aPos.z*0.003, 1.0);
}

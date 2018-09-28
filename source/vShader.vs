#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
   gl_Position = vec4(aPos.x*0.4, aPos.y*0.4, aPos.z*0.4, 1.0);
}

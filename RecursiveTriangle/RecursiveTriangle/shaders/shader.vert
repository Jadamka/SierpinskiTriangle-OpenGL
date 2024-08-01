#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 trans;

void main()
{
	gl_Position = trans * vec4(aPos, 1.0f);
}
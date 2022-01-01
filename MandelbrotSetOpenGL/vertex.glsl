#version 450 core
layout (location = 0) in vec2 aPos;

out vec4 Coords;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	Coords = vec4(aPos, 0.0f, 1.0f);
}
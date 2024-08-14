#version 330 core

layout (location = 0)in vec4 position;
layout (location = 0)in vec2 UVcoord;

out vec2 v_UVcoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_UVcoord = UVcoord;
};

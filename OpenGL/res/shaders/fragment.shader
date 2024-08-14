#version 330 core

layout (location = 0)out vec4 color;

in vec2 v_UVcoord;

uniform vec3 u_Color;

void main()
{
	float distance = length(v_UVcoord);

	float alpha = 1;

	if(distance > 1){
		alpha = 0;
	}

	color = vec4(u_Color.x,u_Color.y,u_Color.z,alpha);

};
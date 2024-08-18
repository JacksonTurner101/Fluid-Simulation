#version 330 core

layout (location = 0)out vec4 color;

in vec2 v_UVcoord;

void main()
{
	float distance = length(v_UVcoord);

	float alpha = 1;

	if(distance > 1){
		alpha = 0;
	}

	color = vec4(1.0f,0.0f,0.0f,alpha);

};
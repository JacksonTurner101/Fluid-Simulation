#version 330 core

out vec4 FragColor;

in vec2 v_TexCoord;

uniform sampler2D u_DensityTexture;

void main()
{
    float density = texture(u_DensityTexture, v_TexCoord).r;
    FragColor = vec4(density, density, density, 1.0);
}
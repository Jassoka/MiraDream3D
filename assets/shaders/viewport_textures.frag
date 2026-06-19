#version 330 core

uniform sampler2D uTexture;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture, vec2(0.5, 0.5));
}
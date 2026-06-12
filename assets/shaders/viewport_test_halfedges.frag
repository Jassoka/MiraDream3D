#version 330 core

out vec4 fragColor;
in vec3 color;
void main()
{
    if (color.x + color.z < 1.0)
        fragColor = vec4(0.8, 0.8, 0.8 ,1.0);
    else
        fragColor = vec4(vec3(1.0,0.35,0.0) ,1.0);
}
#version 330 core
out vec4 fragColor;

uniform vec3 backgroundColor;
void main()
{
    fragColor = vec4(backgroundColor, 1.0);
}

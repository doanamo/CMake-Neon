#version 330 core

uniform mat4 vertTransform;
layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec3 vertColor;
out vec4 fragColor;

void main()
{
    gl_Position = vertTransform * vec4(vertPosition, 1.0f);
    fragColor = vec4(vertColor, 1.0f);
}

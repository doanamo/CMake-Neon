#version 330 core

uniform mat4 vertTransform;
layout(location = 0) in vec3 vertPosition;
layout(location = 1) in vec2 vertTexture;
out vec2 fragTexture;

void main()
{
    gl_Position = vertTransform * vec4(vertPosition, 1.0f);
    fragTexture = vertTexture;
}

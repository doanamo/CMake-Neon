#version 330 core

in vec2 fragTexture;
out vec4 finalColor;

uniform sampler2D textureSampler;

void main()
{
    finalColor = texture(textureSampler, fragTexture);
}

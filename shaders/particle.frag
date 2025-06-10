#version 330 core

in vec2 texCoords;
out vec4 FragColor;

uniform vec4 particleColor;
uniform sampler2D tex;

void main(){
    FragColor = texture(tex, texCoords) * particleColor;
}
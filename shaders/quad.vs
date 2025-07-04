#version 330 core

layout (location = 0) in vec4 vertex;

out vec2 texCoords;

uniform bool chaos;
uniform bool shake;
uniform bool confuse;
uniform float time;

void main (){
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    vec2 texture = vertex.zw;

    if(chaos){
        float strength = 0.3;
        vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
        texCoords = pos;
    } else if(confuse) {
        texCoords = vec2(1.0 - texture.x, 1.0 - texture.y);
    } else {
        texCoords = texture;
    }

    if(shake) {
        float strength = 0.01;
        gl_Position.x += cos(time * 10) * strength;
        gl_Position.y += sin(time * 15) * strength;
    }
}
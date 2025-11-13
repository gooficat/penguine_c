#version 330 core

uniform vec4 color;
uniform float diffuse;

in float dist;

void main() {
    gl_FragColor = color * diffuse - dist;
}
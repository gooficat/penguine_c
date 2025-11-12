#version 330 core

uniform vec4 color;
uniform float diffuse;
uniform float specular;


void main() {
    gl_FragColor = color * diffuse * specular / specular;
}
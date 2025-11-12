#version 330 core

uniform float diffuse;
uniform float specular;


void main() {
    gl_FragColor = vec4(vec3(1.0, 0.2, 0.4) * diffuse * specular / specular, 1.0);
}
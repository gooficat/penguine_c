#version 330 core

uniform vec4 color;
uniform float diffuse;

in float dist;
in vec2 uv;

uniform sampler2D tex;

void main() {
    gl_FragColor = texture(tex, uv);//color * diffuse - dist;
}
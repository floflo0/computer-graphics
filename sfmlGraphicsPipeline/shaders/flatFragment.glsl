#version 400

uniform float time;

in vec4 color;
out vec4 fragmentColor;

void main() {
    fragmentColor = color;
}

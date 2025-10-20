#version 400

uniform float time;

in vec3 normalOut;
out vec4 fragmentColor;

void main() {
    vec3 color = (normalOut + 1.0) * 0.5;
    fragmentColor = vec4(color, 1.0);
}

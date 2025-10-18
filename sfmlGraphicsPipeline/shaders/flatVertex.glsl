#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec3 vColor;
out vec4 color;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(vPosition, 1.0);
    color = vec4(vColor, 1.0);
}

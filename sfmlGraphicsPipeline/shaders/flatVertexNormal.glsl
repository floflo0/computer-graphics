#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec3 vNormal;

out vec3 normalOut;

// in vec3 vcolor;
// out vec4 color;

void main() {
    gl_Position = projMat * viewMat * modelMat * vec4(vPosition, 1.0);
    mat3 normalMatrix = transpose(inverse(mat3(modelMat)));
    normalOut = normalize(normalMatrix * vNormal);
}

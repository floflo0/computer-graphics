#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
out vec4 surfel_color;

void main()
{
    // Transform coordinates from local space to clipped space
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
    surfel_color = vec4(1, 0, 0, 1);
}

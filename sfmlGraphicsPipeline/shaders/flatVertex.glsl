#version 400

uniform mat4 projMat, viewMat, modelMat;

in vec3 vPosition;
in vec3 vColor; // Paramètre d'entrée

out vec4 surfel_color;

void main()
{
    // Transform coordinates from local space to clipped space
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition, 1.0f);
    surfel_color = vec4(vColor, 1); //Assigner la couleur reçue en entrée à la couleur de sortie
}

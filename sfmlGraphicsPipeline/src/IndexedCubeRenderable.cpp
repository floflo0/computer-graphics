#include "./../include/IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram)
    : Renderable(shaderProgram), m_vBuffer(0), m_vColorBuffer(0) {
    m_base_coordinates = {
        {-0.5f, -0.5f, -0.5f},
        {-0.5f,  0.5f, -0.5f},
        { 0.5f,  0.5f, -0.5f},
        { 0.5f, -0.5f, -0.5f},
        { 0.5f, -0.5f,  0.5f},
        { 0.5f,  0.5f,  0.5f},
        {-0.5f,  0.5f,  0.5f},
        {-0.5f, -0.5f,  0.5f},
    };

    glcheck(glGenBuffers(1, &m_vBuffer));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER,
                         m_base_coordinates.size() * sizeof(glm::vec3),
                         m_base_coordinates.data(), GL_STATIC_DRAW));

    m_triangles_indices = {
        // front face
        0, 1, 2,
        0, 2, 3,
        // right face
        3, 2, 5,
        3, 5, 4,
        // back face
        4, 5, 6,
        4, 6, 7,
        // left face
        7, 6, 1,
        7, 1, 0,
        // top face
        1, 6, 5,
        1, 5, 2,
        // bottom face
        7, 0, 3,
        7, 3, 4,
    };

    glcheck(glGenBuffers(1, &m_indicesBuffer));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         m_triangles_indices.size() * sizeof(unsigned int),
                         m_triangles_indices.data(), GL_STATIC_DRAW));

    m_model = glm::mat4(1.0f);

    m_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));  // red
    m_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));  // green
    m_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));  // blue
    m_colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));  // yellow
    m_colors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));  // magenta
    m_colors.push_back(glm::vec3(0.0f, 1.0f, 1.0f));  // cyan
	m_colors.push_back(glm::vec3(0.0f, 0.5f, 0.0f));
	m_colors.push_back(glm::vec3(0.0f, 0.3f, 0.5f));

    glcheck(glGenBuffers(1, &m_vColorBuffer));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER,
                         m_colors.size() * sizeof(glm::vec3),
                         m_colors.data(), GL_STATIC_DRAW));
  }

void IndexedCubeRenderable::do_draw() {
    // Get the identifier (location) of the uniform modelMat in the shader program
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    // Send the data corresponding to this identifier on the GPU
    glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE,
                               glm::value_ptr(m_model)));

    // Get the identifier of the attribute vPosition in the shader program
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    // Activate the attribute array at this location
    glcheck(glEnableVertexAttribArray(positionLocation));
    // Bind the position buffer on the GL_ARRAY_BUFFER target
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    // Specify the location and the format of the vertex position attribute
    glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0,
                                  (void*)0));

    const int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    glcheck(glEnableVertexAttribArray(colorLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
    glcheck(glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0,
                                  (void*)0));

    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indicesBuffer));
    // Draw the triangles
    glcheck(glDrawElements(GL_TRIANGLES, m_triangles_indices.size(),
                           GL_UNSIGNED_INT, 0));

    // Release the vertex attribute array
    glcheck(glDisableVertexAttribArray(positionLocation));
    // Release the vertex color array
    glcheck(glDisableVertexAttribArray(colorLocation));
}

IndexedCubeRenderable::~IndexedCubeRenderable() {
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_vColorBuffer));
    glcheck(glDeleteBuffers(1, &m_indicesBuffer));
}

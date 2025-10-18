#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram)
  : Renderable(shaderProgram), m_vBuffer(0), m_vColorBuffer(0) {
    // Build the geometry : just a simple triangle for now.
    m_positions.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
    m_positions.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
    m_positions.push_back(glm::vec3(0.0f, 1.0f, 0.0f));

    m_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));  // red
    m_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));  // green
    m_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));  // blue

    // Set the model matrix to identity.
    m_model = glm::mat4(1.0f);

    // Create buffers
    glcheck(glGenBuffers(1, &m_vBuffer));  // vertices

    // Activate buffer and send data to the graphics card.
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER,
                         m_positions.size() * sizeof(glm::vec3),
                         m_positions.data(), GL_STATIC_DRAW));

    glcheck(glGenBuffers(1, &m_vColorBuffer));  // colors
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER,
                         m_colors.size() * sizeof(glm::vec3),
                         m_colors.data(), GL_STATIC_DRAW));
  }

PFNGLBINDBUFFERPROC glBindBuffer;


void CubeRenderable::do_draw() {
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

    // Draw the triangles
    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));

    // Release the vertex attribute array
    glcheck(glDisableVertexAttribArray(positionLocation));
    // Release the vertex color array
    glcheck(glDisableVertexAttribArray(colorLocation));
}

CubeRenderable::~CubeRenderable() {
    glcheck(glDeleteBuffers(1, &m_vBuffer));
}

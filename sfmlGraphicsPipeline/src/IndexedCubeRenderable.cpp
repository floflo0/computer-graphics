#include "./../include/IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"


#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram)
  : Renderable(shaderProgram), m_vBuffer(0), m_vColorBuffer(0), m_iBuffer(0)
{

	// Exercice 3 : Geometry with indexing

	base_coord = {
		{0, 0, 0}, // 0
		{1, 0, 0}, // 1
		{0, 1, 0}, // 2
		{0, 0, 1}, // 3
		{1, 1, 0}, // 4
		{1, 0, 1}, // 5
		{0, 1, 1}, // 6
		{1, 1, 1}  // 7
	};

	// Buffer for coordinates 
	glGenBuffers(1, &m_vBuffer); 
	glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer);
	glBufferData(GL_ARRAY_BUFFER, base_coord.size()*sizeof(glm::vec3), base_coord.data(), GL_STATIC_DRAW);


	// Indices corresponding to all triangles
triangles_indices = {
    // Front (z = 0)
    0, 1, 2,
    2, 1, 4,

    // Back (z = 1)
    3, 6, 5,
    6, 7, 5,

    // Left (x = 0)
    0, 2, 3,
    2, 6, 3,

    // Right (x = 1)
    1, 5, 4,
    4, 5, 7,

    // Bottom (y = 0)
    0, 3, 1,
    1, 3, 5,

    // Top (y = 1)
    2, 4, 6,
    4, 7, 6
};


	// Buffer for indices
	glcheck(glGenBuffers(1, &m_iBuffer));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, triangles_indices.size() * sizeof(unsigned int), triangles_indices.data(), GL_STATIC_DRAW));

	
	// Set the model matrix to identity
	m_model = glm::mat4(1.0);
	
	// 8 vertices so 8 colors
	m_colors.push_back(glm::vec3(1, 0, 0));  // red
	m_colors.push_back(glm::vec3(0, 1, 0));  // green
	m_colors.push_back(glm::vec3(0, 0, 1));  // blue
	m_colors.push_back(glm::vec3(1, 1, 0)); // jaune
	m_colors.push_back(glm::vec3(1, 0, 1));  // magenta
	m_colors.push_back(glm::vec3(0, 1, 1)); 
	m_colors.push_back(glm::vec3(0, 0.5f, 0)); 
	m_colors.push_back(glm::vec3(0, 0.3f, 0.5f)); 


	// Buffer for colors
	glcheck(glGenBuffers(1, &m_vColorBuffer));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vColorBuffer));
	glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW));

}

void IndexedCubeRenderable::do_draw()
{
	// Get the identifier ( location ) of the uniform modelMat in the shader program
	int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
	// Send the data corresponding to this identifier on the GPU
	glUniformMatrix4fv( modelLocation , 1, GL_FALSE , glm::value_ptr( m_model ));

	
	// Get the identifier of the attribute vPosition in the shader program
	int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
	// Activate the attribute array at this location
	glEnableVertexAttribArray( positionLocation );
	// Bind the position buffer on the GL_ARRAY_BUFFER target
	glBindBuffer( GL_ARRAY_BUFFER , m_vBuffer );
	// Specify the location and the format of the vertex position attribute
	glVertexAttribPointer( positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	

	const int color_location = m_shaderProgram->getAttributeLocation("vColor");

	glcheck(glEnableVertexAttribArray(color_location));
	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vColorBuffer));
	glcheck(glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));


	// Draw the triangles
	glDrawElements(GL_TRIANGLES, triangles_indices.size(), GL_UNSIGNED_INT, 0) ;

	// Release the vertex attribute array
	//glDisableVertexAttribArray( positionLocation );

	glDisableVertexAttribArray( color_location );
	glDisableVertexAttribArray( positionLocation );
}

IndexedCubeRenderable::~IndexedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
}

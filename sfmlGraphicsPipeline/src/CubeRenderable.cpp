#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"


#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram)
  : Renderable(shaderProgram), m_vBuffer(0), m_vColorBuffer(0)
{

	/* Exercice 1
	// Build the geometry : just a simple triangle for now
	m_positions.push_back( glm::vec3 (-1 ,0 ,0) );
	m_positions.push_back( glm::vec3 (1 ,0 ,0) );
	m_positions.push_back( glm::vec3 (0 ,1 ,0) );

	glGenBuffers(1, &m_vBuffer); //vertices

	//Activate buffer and send data to the graphics card
	glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
	
	// Set the model matrix to identity
	m_model = glm::mat4(1.0);

	// List of colors for the vertices
	std::vector<glm::vec3> m_colors;
	
	m_colors.push_back(glm::vec3(1.0f, 0.0f, 0.0f));  // red
	m_colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));  // green
	m_colors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));  // blue

	//Create buffers
	glcheck(glGenBuffers(1, &m_vColorBuffer));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
	glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW));

	*/
	

	// Exercice 2 : Creation of a cube

	std::vector< glm::vec3 > normals;
	std::vector< glm::vec2 > tcoords;

	// Call the getUnitCube fonction that will fill its 3 parameters
	getUnitCube(m_positions, normals, tcoords);

	// std::cout << "Vertices" << normals.size() << std::endl ;

	glGenBuffers(1, &m_vBuffer); //vertices

	//Activate buffer and send data to the graphics card
	glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW);
	
	// Set the model matrix to identity
	m_model = glm::mat4(1.0);

	// List of colors for the vertices
	std::vector<glm::vec3> m_colors;
	
    // Exemple : une couleur différente par triangle
    for (size_t i = 0; i < m_positions.size(); i += 3)
    {
        glm::vec3 color;
        switch ((i / 3) % 6) { // 6 faces
            case 0: color = glm::vec3(1, 0, 0); break; // rouge
            case 1: color = glm::vec3(0, 1, 0); break; // vert
            case 2: color = glm::vec3(0, 0, 1); break; // bleu
            case 3: color = glm::vec3(1, 1, 0); break; // jaune
            case 4: color = glm::vec3(1, 0, 1); break; // magenta
            case 5: color = glm::vec3(0, 1, 1); break; // cyan
        }
        // Trois fois la même couleur par triangle
        m_colors.push_back(color);
        m_colors.push_back(color);
        m_colors.push_back(color);
    }

	//Create buffers
	glcheck(glGenBuffers(1, &m_vColorBuffer));
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
	glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec3), m_colors.data(), GL_STATIC_DRAW));

}

void CubeRenderable::do_draw()
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
	glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vColorBuffer));
	glcheck(glVertexAttribPointer(color_location, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

	// Draw the triangles
	glDrawArrays( GL_TRIANGLES, 0, m_positions.size());

	// Release the vertex attribute array
	//glDisableVertexAttribArray( positionLocation );

	glDisableVertexAttribArray( color_location );
	glDisableVertexAttribArray( positionLocation );
}

CubeRenderable::~CubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
}

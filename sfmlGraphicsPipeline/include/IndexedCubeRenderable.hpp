#ifndef INDEXED_CUBE_RENDERABLE_HPP
#define INDEXED_CUBE_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class IndexedCubeRenderable : public Renderable
{
    public:
        ~IndexedCubeRenderable();
        IndexedCubeRenderable( ShaderProgramPtr program );

    private:
        void do_draw();

        unsigned int m_vBuffer;
        unsigned int m_vColorBuffer;
        unsigned int m_iBuffer;

    	std::vector< glm::vec3 > base_coord;
	    std::vector< unsigned int > triangles_indices;
	    std::vector<glm::vec3> m_colors; 	// List of colors for each vertices

        glm::mat4 m_model;
};

typedef std::shared_ptr<IndexedCubeRenderable> IndexedCubeRenderablePtr;

#endif

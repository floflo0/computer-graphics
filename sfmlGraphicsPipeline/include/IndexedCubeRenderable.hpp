#ifndef INDEXED_CUBE_RENDERABLE_HPP
#define INDEXED_CUBE_RENDERABLE_HPP

#include "Renderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class IndexedCubeRenderable : public Renderable
{
    public:
        ~IndexedCubeRenderable();
        IndexedCubeRenderable(ShaderProgramPtr program);

    private:
        void do_draw();

        std::vector<glm::vec3> m_base_coordinates;
        std::vector<unsigned int> m_triangles_indices;
        std::vector<glm::vec3> m_colors;
        unsigned int m_vBuffer;
        unsigned int m_vColorBuffer;
        unsigned int m_indicesBuffer;

        glm::mat4 m_model;
};

typedef std::shared_ptr<IndexedCubeRenderable> IndexedCubeRenderablePtr;

#endif  // INDEXED_CUBE_RENDERABLE_HPP

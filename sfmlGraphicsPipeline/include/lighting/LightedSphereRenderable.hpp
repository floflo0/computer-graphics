#ifndef LIGHTED_SPHERE_RENDERABLE_HPP
#define LIGHTED_SPHERE_RENDERABLE_HPP

#include "./../lighting/LightedMeshRenderable.hpp"
#include "./../lighting/Material.hpp"
#include <vector>
#include <glm/glm.hpp>

class LightedSphereRenderable : public LightedMeshRenderable
{
public:
    ~LightedSphereRenderable();
    LightedSphereRenderable(ShaderProgramPtr prog, bool indexed, const MaterialPtr & mat, unsigned int strips, unsigned int slice=30u, bool vertex_normals=true);
};

typedef std::shared_ptr<LightedSphereRenderable> LightedSphereRenderablePtr;

#endif

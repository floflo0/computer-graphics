#pragma once

#include <texturing/TexturedLightedMeshRenderable.hpp>

class Kart {
public:
    Kart(ShaderProgramPtr shader_program, const MaterialPtr &material,
         const float scale);

    TexturedLightedMeshRenderablePtr getRenderable() const;

private:
    TexturedLightedMeshRenderablePtr car;
    TexturedLightedMeshRenderablePtr canon;
    TexturedLightedMeshRenderablePtr chain;
    TexturedLightedMeshRenderablePtr eyes;
    TexturedLightedMeshRenderablePtr volant_centre;
    TexturedLightedMeshRenderablePtr volant_tour;
    TexturedLightedMeshRenderablePtr volant_barre;
    TexturedLightedMeshRenderablePtr gouvernaille;
    TexturedLightedMeshRenderablePtr cylinder_back;
    TexturedLightedMeshRenderablePtr cylinder_front;
    TexturedLightedMeshRenderablePtr head_back;
    TexturedLightedMeshRenderablePtr hair_head_back;
    TexturedLightedMeshRenderablePtr head_front;
    TexturedLightedMeshRenderablePtr tire_back_right;
    TexturedLightedMeshRenderablePtr tire_back_left;
    TexturedLightedMeshRenderablePtr tire_front_right;
    TexturedLightedMeshRenderablePtr tire_front_left;
};

typedef std::shared_ptr<Kart> KartPtr;

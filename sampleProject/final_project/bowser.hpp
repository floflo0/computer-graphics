#pragma once

#include <texturing/TexturedLightedMeshRenderable.hpp>

class Bowser {
public:
    Bowser(ShaderProgramPtr shader_program, const MaterialPtr &material,
           const float scale);

    TexturedLightedMeshRenderablePtr getRenderable() const;

private:
    TexturedLightedMeshRenderablePtr body;
    TexturedLightedMeshRenderablePtr arm_right;
    TexturedLightedMeshRenderablePtr arm_left;
    TexturedLightedMeshRenderablePtr leg_right;
    TexturedLightedMeshRenderablePtr leg_left;
    TexturedLightedMeshRenderablePtr tail;
    TexturedLightedMeshRenderablePtr eyes;
};

typedef std::shared_ptr<Bowser> BowserPtr;

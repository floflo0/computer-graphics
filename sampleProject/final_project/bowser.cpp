#include "bowser.hpp"

#include <Utils.hpp>

const std::string mesh_body_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_body.obj";
const std::string mesh_arm_right_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_arm_right.obj";
const std::string mesh_arm_left_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_arm_left.obj";
const std::string mesh_leg_right_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_leg_right.obj";
const std::string mesh_leg_left_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_leg_left.obj";
const std::string mesh_tail_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_tail.obj";
const std::string mesh_eyes_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_eyes.obj";

const std::string main_texture = "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png";
const std::string eyes_texture = "../../sfmlGraphicsPipeline/textures/bowser/bowser_eyes.png";

#define ADD_CHILD_MESH(part, texture) do {                               \
        this->part = std::make_shared<TexturedLightedMeshRenderable>(    \
            shader_program,                                              \
            mesh_##part##_path,                                          \
            material,                                                    \
            texture                                                      \
        );                                                               \
        TexturedLightedMeshRenderable::addChild(this->body, this->part); \
    } while (0)

Bowser::Bowser(ShaderProgramPtr shader_program, const MaterialPtr &material,
               const float scale) {
    this->body = std::make_shared<TexturedLightedMeshRenderable>(
        shader_program,
        mesh_body_path,
        material,
        main_texture
    );
    this->body->setGlobalTransform(getScaleMatrix(scale));
    ADD_CHILD_MESH(arm_right, main_texture);
    ADD_CHILD_MESH(arm_left, main_texture);
    ADD_CHILD_MESH(leg_right, main_texture);
    ADD_CHILD_MESH(leg_left, main_texture);
    ADD_CHILD_MESH(tail, main_texture);
    ADD_CHILD_MESH(eyes, eyes_texture);
}

TexturedLightedMeshRenderablePtr Bowser::getRenderable() const {
    return this->body;
}

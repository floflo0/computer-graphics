#include "bowser.hpp"

#include <SkeletonRenderable.hpp>
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

#define ADD_CHILD_MESH(part, texture, parent) do {                    \
        this->part = std::make_shared<TexturedLightedMeshRenderable>( \
            shader_program,                                           \
            mesh_##part##_path,                                       \
            material,                                                 \
            texture                                                   \
        );                                                            \
        TexturedLightedMeshRenderable::addChild(parent, this->part);  \
    } while (0)

Bowser::Bowser(ShaderProgramPtr shader_program, const MaterialPtr &material,
               const glm::vec3 position, const float scale) {
    this->body = std::make_shared<TexturedLightedMeshRenderable>(
        shader_program,
        mesh_body_path,
        material,
        main_texture
    );
    this->body->setGlobalTransform(getTranslationMatrix(position) *
                                   getScaleMatrix(scale));

    const auto right_arm_origin = glm::vec3(3.5f, -13.0f, -3.0f);
    const auto left_arm_origin = glm::vec3(-3.5f, -13.0f, -3.0f);
    const auto right_leg_origin = glm::vec3(3.0f, -7.0f, 0.0f);
    const auto left_leg_origin = glm::vec3(-3.0f, -7.0f, 0.0f);

    auto right_shoudler = std::make_shared<SkeletonRenderable>(shader_program);
    right_shoudler->setGlobalTransform(getTranslationMatrix(-right_arm_origin));
    HierarchicalRenderable::addChild(this->body, right_shoudler);

    auto left_shoudler = std::make_shared<SkeletonRenderable>(shader_program);
    left_shoudler->setGlobalTransform(getTranslationMatrix(-left_arm_origin));
    HierarchicalRenderable::addChild(this->body, left_shoudler);

    ADD_CHILD_MESH(arm_right, main_texture, right_shoudler);
    ADD_CHILD_MESH(arm_left, main_texture, left_shoudler);
    ADD_CHILD_MESH(leg_right, main_texture, this->body);
    ADD_CHILD_MESH(leg_left, main_texture, this->body);
    ADD_CHILD_MESH(eyes, eyes_texture, this->body);

    this->arm_right->setLocalTransform(getTranslationMatrix(right_arm_origin));
    const auto right_arm_tranform1 = GeometricTransformation(
        glm::vec3(0.0f),
        qY(glm::radians(75.0f)) *
        qZ(glm::radians(20.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    );

    const auto right_arm_tranform2 = GeometricTransformation(
        glm::vec3(0.0f),
        qY(glm::radians(85.0f)) *
        qZ(glm::radians(0.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    );
    this->arm_right->addGlobalTransformKeyframe(right_arm_tranform1, 0.0f);
    this->arm_right->addGlobalTransformKeyframe(right_arm_tranform2, 1.0f);
    this->arm_right->addGlobalTransformKeyframe(right_arm_tranform1, 2.0f);

    this->arm_left->setLocalTransform(getTranslationMatrix(left_arm_origin));
    const auto left_arm_tranform1 = GeometricTransformation(
        glm::vec3(0.0f),
        qY(glm::radians(-85.0f)) *
        qZ(glm::radians(0.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    );
    const auto left_arm_tranform2 = GeometricTransformation(
        glm::vec3(0.0f),
        qY(glm::radians(-75.0f)) *
        qZ(glm::radians(-20.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    );
    this->arm_left->addGlobalTransformKeyframe(left_arm_tranform1, 0.0f);
    this->arm_left->addGlobalTransformKeyframe(left_arm_tranform2, 1.0f);
    this->arm_left->addGlobalTransformKeyframe(left_arm_tranform1, 2.0f);

    this->leg_right->setGlobalTransform(
        getTranslationMatrix(-right_leg_origin) *
        getRotationMatrix(glm::radians(-70.0f), 1.0f, 0.0f, 0.0f) *
        getTranslationMatrix(right_leg_origin)
    );

    this->leg_left->setGlobalTransform(
        getTranslationMatrix(-left_leg_origin) *
        getRotationMatrix(glm::radians(-70.0f), 1.0f, 0.0f, 0.0f) *
        getTranslationMatrix(left_leg_origin)
    );
}

TexturedLightedMeshRenderablePtr Bowser::getRenderable() const {
    return this->body;
}

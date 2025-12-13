#include "kart.hpp"

#include <SkeletonRenderable.hpp>
#include <Utils.hpp>

#define TIRE_ROTATION_SPEED 0.2f  // 5 turns/s

const std::string mesh_car_path = "../../sfmlGraphicsPipeline/meshes/kart/car.obj";
const std::string mesh_canon_path = "../../sfmlGraphicsPipeline/meshes/kart/canon.obj";
const std::string mesh_chain_path = "../../sfmlGraphicsPipeline/meshes/kart/chain.obj";
const std::string mesh_eyes_path = "../../sfmlGraphicsPipeline/meshes/kart/eyes.obj";
const std::string mesh_gouvernaille_path = "../../sfmlGraphicsPipeline/meshes/kart/gouvernaille.obj";
const std::string mesh_cylinder_back_path = "../../sfmlGraphicsPipeline/meshes/kart/cylinder_back.obj";
const std::string mesh_cylinder_front_path = "../../sfmlGraphicsPipeline/meshes/kart/cylinder_front.obj";
const std::string mesh_head_back_path = "../../sfmlGraphicsPipeline/meshes/kart/head_back.obj";
const std::string mesh_hair_head_back_path = "../../sfmlGraphicsPipeline/meshes/kart/hair_head_back.obj";
const std::string mesh_head_front_path = "../../sfmlGraphicsPipeline/meshes/kart/head_front.obj";
const std::string mesh_tire_back_right_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_back_right.obj";
const std::string mesh_tire_back_left_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_back_left.obj";
const std::string mesh_tire_front_right_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_front_right.obj";
const std::string mesh_tire_front_left_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_front_left.obj";

const std::string main_texture = "../../sfmlGraphicsPipeline/textures/kart/kart.png";
const std::string eyes_texture = "../../sfmlGraphicsPipeline/textures/kart/eyes.png";

#define ADD_CHILD_MESH(part, texture, parent) do {                    \
        this->part = std::make_shared<TexturedLightedMeshRenderable>( \
            shader_program,                                           \
            mesh_##part##_path,                                       \
            material,                                                 \
            texture                                                   \
        );                                                            \
        TexturedLightedMeshRenderable::addChild(parent, this->part);  \
    } while (0)

Kart::Kart(ShaderProgramPtr shader_program, const MaterialPtr &material,
           const float scale) {
    this->car = std::make_shared<TexturedLightedMeshRenderable>(
        shader_program,
        mesh_car_path,
        material,
        main_texture
    );
    this->car->setGlobalTransform(getScaleMatrix(scale));

    const auto tire_back_right_origin = glm::vec3(0.0f, -3.5f, 1.5f);
    const auto tire_back_left_origin = glm::vec3(0.0f, -3.5f, 1.5f);
    const auto tire_front_right_origin = glm::vec3(0.0f, -2.5f, -10.85f);
    const auto tire_front_left_origin = glm::vec3(0.0f, -2.5f, -10.85f);
    auto axle_back_right = std::make_shared<SkeletonRenderable>(shader_program);
    auto axle_back_left = std::make_shared<SkeletonRenderable>(shader_program);
    auto axle_front_right = std::make_shared<SkeletonRenderable>(shader_program);
    auto axle_front_left = std::make_shared<SkeletonRenderable>(shader_program);
    HierarchicalRenderable::addChild(this->car, axle_back_right);
    HierarchicalRenderable::addChild(this->car, axle_back_left);
    HierarchicalRenderable::addChild(this->car, axle_front_right);
    HierarchicalRenderable::addChild(this->car, axle_front_left);
    axle_back_right->setGlobalTransform(getTranslationMatrix(-tire_back_right_origin));
    axle_back_left->setGlobalTransform(getTranslationMatrix(-tire_back_left_origin));
    axle_front_right->setGlobalTransform(getTranslationMatrix(-tire_front_right_origin));
    axle_front_left->setGlobalTransform(getTranslationMatrix(-tire_front_left_origin));

    ADD_CHILD_MESH(canon, main_texture, this->car);
    ADD_CHILD_MESH(chain, main_texture, this->car);
    ADD_CHILD_MESH(eyes, eyes_texture, this->car);
    ADD_CHILD_MESH(gouvernaille, main_texture, this->car);
    ADD_CHILD_MESH(cylinder_back, main_texture, this->car);
    ADD_CHILD_MESH(cylinder_front, main_texture, this->car);
    ADD_CHILD_MESH(head_back, main_texture, this->car);
    ADD_CHILD_MESH(hair_head_back, main_texture, this->car);
    ADD_CHILD_MESH(head_front, main_texture, this->car);
    ADD_CHILD_MESH(tire_back_right, main_texture, axle_back_right);
    ADD_CHILD_MESH(tire_back_left, main_texture, axle_back_left);
    ADD_CHILD_MESH(tire_front_right, main_texture, axle_front_right);
    ADD_CHILD_MESH(tire_front_left, main_texture, axle_front_left);

    const auto tire_roation1 = GeometricTransformation(
        glm::vec3(0.0f),
        qX(0.0f),
        glm::vec3(1.0f)
    );
    const auto tire_roation2 = GeometricTransformation(
        glm::vec3(0.0f),
        qX(glm::pi<float>() / 2.0f),
        glm::vec3(1.0f)
    );
    const auto tire_roation3 = GeometricTransformation(
        glm::vec3(0.0f),
        qX(glm::pi<float>()),
        glm::vec3(1.0f)
    );
    const auto tire_roation4 = GeometricTransformation(
        glm::vec3(0.0f),
        qX(-glm::pi<float>() / 2.0f),
        glm::vec3(1.0f)
    );

    this->tire_back_right->setLocalTransform(getTranslationMatrix(tire_back_right_origin));
    this->tire_back_left->setLocalTransform(getTranslationMatrix(tire_back_left_origin));
    this->tire_front_right->setLocalTransform(getTranslationMatrix(tire_front_right_origin));
    this->tire_front_left->setLocalTransform(getTranslationMatrix(tire_front_left_origin));

    this->tire_back_right->addGlobalTransformKeyframe(tire_roation1, 0.0f);
    this->tire_back_right->addGlobalTransformKeyframe(tire_roation2, 0.25f * TIRE_ROTATION_SPEED);
    this->tire_back_right->addGlobalTransformKeyframe(tire_roation3, 0.5f * TIRE_ROTATION_SPEED);
    this->tire_back_right->addGlobalTransformKeyframe(tire_roation4, 0.75f * TIRE_ROTATION_SPEED);
    this->tire_back_right->addGlobalTransformKeyframe(tire_roation1, TIRE_ROTATION_SPEED);
    this->tire_back_left->addGlobalTransformKeyframe(tire_roation1, 0.0f);
    this->tire_back_left->addGlobalTransformKeyframe(tire_roation2, 0.25f * TIRE_ROTATION_SPEED);
    this->tire_back_left->addGlobalTransformKeyframe(tire_roation3, 0.5f * TIRE_ROTATION_SPEED);
    this->tire_back_left->addGlobalTransformKeyframe(tire_roation4, 0.75f * TIRE_ROTATION_SPEED);
    this->tire_back_left->addGlobalTransformKeyframe(tire_roation1, TIRE_ROTATION_SPEED);
    this->tire_front_right->addGlobalTransformKeyframe(tire_roation1, 0.0f);
    this->tire_front_right->addGlobalTransformKeyframe(tire_roation2, 0.25f * TIRE_ROTATION_SPEED);
    this->tire_front_right->addGlobalTransformKeyframe(tire_roation3, 0.5f * TIRE_ROTATION_SPEED);
    this->tire_front_right->addGlobalTransformKeyframe(tire_roation4, 0.75f * TIRE_ROTATION_SPEED);
    this->tire_front_right->addGlobalTransformKeyframe(tire_roation1, TIRE_ROTATION_SPEED);
    this->tire_front_left->addGlobalTransformKeyframe(tire_roation1, 0.0f);
    this->tire_front_left->addGlobalTransformKeyframe(tire_roation2, 0.25f * TIRE_ROTATION_SPEED);
    this->tire_front_left->addGlobalTransformKeyframe(tire_roation3, 0.5f * TIRE_ROTATION_SPEED);
    this->tire_front_left->addGlobalTransformKeyframe(tire_roation4, 0.75f * TIRE_ROTATION_SPEED);
    this->tire_front_left->addGlobalTransformKeyframe(tire_roation1, TIRE_ROTATION_SPEED);
}

TexturedLightedMeshRenderablePtr Kart::getRenderable() const {
    return this->car;
}

#include "kart.hpp"

#include <Utils.hpp>

const std::string mesh_car_path = "../../sfmlGraphicsPipeline/meshes/kart/car.obj";
const std::string mesh_canon_path = "../../sfmlGraphicsPipeline/meshes/kart/canon.obj";
const std::string mesh_chain_path = "../../sfmlGraphicsPipeline/meshes/kart/chain.obj";
const std::string mesh_eyes_path = "../../sfmlGraphicsPipeline/meshes/kart/eyes.obj";
const std::string mesh_volant_centre_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_centre.obj";
const std::string mesh_volant_tour_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_tour.obj";
const std::string mesh_volant_barre_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_barre.obj";
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

#define ADD_CHILD_MESH(part, texture) do {                              \
        this->part = std::make_shared<TexturedLightedMeshRenderable>(   \
            shader_program,                                             \
            mesh_##part##_path,                                         \
            material,                                                   \
            texture                                                     \
        );                                                              \
        TexturedLightedMeshRenderable::addChild(this->car, this->part); \
    } while (0)

Kart::Kart(ShaderProgramPtr shader_program, const MaterialPtr &material,
           const float scale) {
    this->car = std::make_shared<TexturedLightedMeshRenderable>(
        shader_program,
        mesh_car_path,
        material,
        main_texture
    );
    this->car->setGlobalTransform(getTranslationMatrix(-2.5f, 0.0f, 0.0f) *
                                  getScaleMatrix(scale));
    ADD_CHILD_MESH(canon, main_texture);
    ADD_CHILD_MESH(chain, main_texture);
    ADD_CHILD_MESH(eyes, eyes_texture);
    ADD_CHILD_MESH(volant_centre, main_texture);
    ADD_CHILD_MESH(volant_tour, main_texture);
    ADD_CHILD_MESH(volant_barre, main_texture);
    ADD_CHILD_MESH(gouvernaille, main_texture);
    ADD_CHILD_MESH(cylinder_back, main_texture);
    ADD_CHILD_MESH(cylinder_front, main_texture);
    ADD_CHILD_MESH(head_back, main_texture);
    ADD_CHILD_MESH(hair_head_back, main_texture);
    ADD_CHILD_MESH(head_front, main_texture);
    ADD_CHILD_MESH(tire_back_right, main_texture);
    ADD_CHILD_MESH(tire_back_left, main_texture);
    ADD_CHILD_MESH(tire_front_right, main_texture);
    ADD_CHILD_MESH(tire_front_left, main_texture);
}

TexturedLightedMeshRenderablePtr Kart::getRenderable() const {
    return this->car;
}

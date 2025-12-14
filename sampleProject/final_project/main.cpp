#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <cstdio>
#include <GeometricTransformation.hpp>
#include <FrameRenderable.hpp>
#include <Utils.hpp>
#include <memory>
#include <texturing/TexturedMeshRenderable.hpp>
#include <Viewer.hpp>

#include <dynamics/BobOmbExplosion.hpp>
#include <dynamics/EulerExplicitSolver.hpp>

#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>
#include <lighting/LightedCylinderRenderable.hpp>
#include <lighting/LightedSphereRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/PointLightRenderable.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <lighting/Material.hpp>

#include <texturing/CubeMapRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/TexturedMeshRenderable.hpp>

#include "bowser.hpp"
#include "kart.hpp"

#define RACE_START_TIME 15.0f  // seconds

KartPtr kart;
TexturedLightedMeshRenderablePtr steel_driver;

MaterialPtr redMat;
MaterialPtr greenMat;
MaterialPtr offMat;

LightedSphereRenderablePtr leftFire;
LightedSphereRenderablePtr middleFire;
LightedSphereRenderablePtr rightFire;

void kartBowser_animation(std::shared_ptr<SkeletonRenderable> &kart_root);
void kartPenguin_animation(Viewer& viewer, TexturedLightedMeshRenderablePtr& steel_driver);
void movingBobomb(Viewer& viewer, TexturedLightedMeshRenderablePtr &bobOmb);

float lap2_start_time;



static float camera_intro_animation(std::shared_ptr<Camera> camera) {
    camera->m_globalKeyframes.clear();

    float camera_animation_time = 0.0f;

    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(0.0f, 0.0f, 10.0f),
        qY(glm::radians(-70.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(10.0f, 10.0f, 10.0f),
        qX(glm::radians(10.0f)) *
        qY(glm::radians(-10.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(45.0f, 20.0f, -20.0f),
        qY(glm::radians(-90.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(10.0f, 20.0f, -20.0f),
        qY(glm::radians(-90.0f)) *
        qX(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(0.0f, 10.0f, -45.0f),
        qY(glm::radians(-150.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(30.0f, 10.0f, -60.0f),
        qY(glm::radians(-180.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(20.0f, 1.8f, -1.6f),
        qY(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(GeometricTransformation(
        glm::vec3(27.0f, 1.8f, -1.6f),
        qY(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    ), camera_animation_time);

    return camera_animation_time;
}

static void camera_animation_follow_kart(std::shared_ptr<Camera> camera, float camera_animation_time) {
    camera->m_globalKeyframes.clear();
    HierarchicalRenderable::addChild(kart->getRenderable(), camera);

    const auto behind_kart = GeometricTransformation(
        glm::vec3(0.0f, 25.0f, -30.0f),
        qY(glm::pi<float>()) *
        qX(glm::radians(-30.0f)),
        glm::vec3(1.0f)
    );

    const auto front_left_kart = GeometricTransformation(
        glm::vec3(20.0f, 20.0f, 30.0f),
        qY(glm::radians(30.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    );

    const auto front_left_close_kart = GeometricTransformation(
        glm::vec3(10.0f, 20.0f, 20.0f),
        qY(glm::radians(30.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    );

    const auto front_right_kart = GeometricTransformation(
        glm::vec3(-20.0f, 20.0f, 30.0f),
        qY(glm::radians(-30.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    );

    const auto front_right_close_kart = GeometricTransformation(
        glm::vec3(-10.0f, 20.0f, 20.0f),
        qY(glm::radians(-30.0f)) *
        qX(glm::radians(-20.0f)),
        glm::vec3(1.0f)
    );

    const auto side_right_kart = GeometricTransformation(
        glm::vec3(-25.0f, 15.0f, 5.0f),
        qY(glm::radians(-90.0f)),
        glm::vec3(1.0f)
    );

    const auto side_left_kart = GeometricTransformation(
        glm::vec3(25.0f, 15.0f, 5.0f),
        qY(glm::radians(90.0f)),
        glm::vec3(1.0f)
    );

    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
    camera_animation_time += 8.0f;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_left_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(front_left_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_right_close_kart, camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(front_right_close_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_left_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(front_left_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(side_right_kart, camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(side_right_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_right_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(front_right_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_left_close_kart, camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(front_left_close_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(front_right_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(front_right_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(side_left_kart, camera_animation_time);
    camera_animation_time += 2.0f;
    camera->addGlobalTransformKeyframe(side_left_kart, camera_animation_time);

    camera_animation_time += 10e-6;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
    camera_animation_time += 3.0f;
    camera->addGlobalTransformKeyframe(behind_kart, camera_animation_time);
}

static void thwomp_animation(TexturedLightedMeshRenderablePtr thwomp) {
    GeometricTransformation thwomp_animation1 = GeometricTransformation(
        glm::vec3(0.0f, 0.0f, 0.0f),
        qX(0),
        glm::vec3(1.0f)
    );
    GeometricTransformation thwomp_animation2 = GeometricTransformation(
        glm::vec3(0.0f, 150.0f, 0.0f),
        qX(0),
        glm::vec3(1.0f)
    );

    thwomp->addLocalTransformKeyframe(thwomp_animation1, 0.0f);
    thwomp->addLocalTransformKeyframe(thwomp_animation1, 1.0f);
    thwomp->addLocalTransformKeyframe(thwomp_animation2, 1.75f);
    thwomp->addLocalTransformKeyframe(thwomp_animation2, 4.75f);
    thwomp->addLocalTransformKeyframe(thwomp_animation1, 5.0f);
}

void initialize_scene(Viewer &viewer) {
    // Create a shader program
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                  "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                     "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
                                                                   "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/simpleTextureVertex.glsl",
                                                                 "../../sfmlGraphicsPipeline/shaders/simpleTextureFragment.glsl");
    ShaderProgramPtr textureShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                     "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(textureShader);
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(cubeMapShader);
    viewer.addShaderProgram(phongShader);

    std::string cubemap_dir = "../../sfmlGraphicsPipeline/textures/skybox2";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);

    viewer.addRenderable(cubemap);

    // auto frame = std::make_shared<FrameRenderable>(flatShader);
    // viewer.addRenderable(frame);
    // frame->setLocalTransform(getScaleMatrix(10,10,10));

    glm::mat4 globalTransformation, localTransformation;


    //lumières directionnelles (globale)
    glm::vec3 d_ambient(2.0f), d_diffuse(1.0f), d_specular(0.5f);

    glm::vec3 d_direction1 = glm::normalize(glm::vec3(-1.0,-1.0,0.0));
    DirectionalLightPtr directionalLight1 = std::make_shared<DirectionalLight>(d_direction1, d_ambient, d_diffuse, d_specular);

    glm::vec3 d_direction2 = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    DirectionalLightPtr directionalLight2 = std::make_shared<DirectionalLight>(d_direction2, d_ambient, d_diffuse, d_specular);

    glm::vec3 d_direction3 = glm::normalize(glm::vec3(1.0,-1.0,0.0));
    DirectionalLightPtr directionalLight3 = std::make_shared<DirectionalLight>(d_direction3, d_ambient, d_diffuse, d_specular);

    viewer.addDirectionalLight(directionalLight1);
    viewer.addDirectionalLight(directionalLight2);
    viewer.addDirectionalLight(directionalLight3);

    // MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
    MaterialPtr myMaterial = std::make_shared<Material>(
            glm::vec3(0.05f), // ambient
            glm::vec3(0.15f), // diffuse
            glm::vec3(1.0f),  // specular
            1.0f
        );


    //------------------Gestion du départ (feu + lakitu)-----------------------------

    // Create Lakitu
    const std::string lakitu_path = "../../sfmlGraphicsPipeline/meshes/lakitu.obj";
    auto lakitu = std::make_shared<TexturedLightedMeshRenderable>(textureShader, lakitu_path, myMaterial, "../../sfmlGraphicsPipeline/textures/lakitu.png");

    lakitu->setGlobalTransform(getTranslationMatrix(24.0f, 3.5f, -5.2f) * getRotationMatrix(M_PI_4f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.02f));
    viewer.addRenderable(lakitu);

    { // Boitier du feu
        auto blackMat = std::make_shared<Material>(
            glm::vec3(0.02f, 0.02f, 0.02f), // ambient (un peu visible)
            glm::vec3(0.05f, 0.05f, 0.05f), // diffuse
            glm::vec3(0.0f),               // specular (mat)
            1.0f
        );

        redMat = std::make_shared<Material>(
            glm::vec3(0.1f, 0.0f, 0.0f), // ambient (un peu visible)
            glm::vec3(1.0f, 0.0f, 0.0f), // diffuse
            glm::vec3(0.0f),               // specular (mat)
            1.0f
        );

        greenMat = std::make_shared<Material>(
            glm::vec3(0.0f, 0.1f, 0.0f), // ambient (un peu visible)
            glm::vec3(0.0f, 1.0f, 0.0f), // diffuse
            glm::vec3(0.0f),               // specular (mat)
            1.0f
        );

        offMat = std::make_shared<Material>(
            glm::vec3(0.0f), // ambient (un peu visible)
            glm::vec3(0.0f), // diffuse
            glm::vec3(0.0f),               // specular (mat)
            1.0f
        );

        auto feuBoitier = std::make_shared<LightedCubeRenderable>(phongShader, false, blackMat);
        feuBoitier -> setGlobalTransform(getTranslationMatrix(glm::vec3(24.1f, 3.0f, -5.2f)) * getRotationMatrix(M_PI_4f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(2.2f, 0.6f, 0.6f));
        viewer.addRenderable(feuBoitier);

        leftFire = std::make_shared<LightedSphereRenderable>(phongShader, false, offMat, 20, 20, true);
        middleFire = std::make_shared<LightedSphereRenderable>(phongShader, false, offMat, 20, 20, true);
        rightFire = std::make_shared<LightedSphereRenderable>(phongShader, false, offMat, 20, 20, true);

        leftFire->setGlobalTransform( getTranslationMatrix(23.75f,3.0f,-4.55f) * getScaleMatrix(0.3f));
        middleFire->setGlobalTransform( getTranslationMatrix(24.25f,3.0f,-5.05f) * getScaleMatrix(0.3f));
        rightFire->setGlobalTransform( getTranslationMatrix(24.75f,3.0f,-5.55f) * getScaleMatrix(0.3f));

        viewer.addRenderable(leftFire);
        viewer.addRenderable(middleFire);
        viewer.addRenderable(rightFire);
    }

    //----------------- Ajout des coureurs ---------------------------

    BowserPtr bowser = std::make_shared<Bowser>(textureShader, myMaterial,
                                                glm::vec3(0.0f, 4.0f, 0.0f),
                                                0.7f);
    // Create Kart
    kart = std::make_shared<Kart>(textureShader, myMaterial);
    kart->getRenderable()->setGlobalTransform(
        getTranslationMatrix(28.5f, 1.17f, -1.6f) *
        getRotationMatrix(-M_PI_2f, 0.0f, 1.0f, 0.0f) *
        getScaleMatrix(0.05f)
    );
    viewer.addRenderable(kart->getRenderable());
    HierarchicalRenderable::addChild(kart->get_car(),
                                     bowser->getRenderable());

    // Create Penguin
    const std::string penguin_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/penguin.obj";
    TexturedLightedMeshRenderablePtr penguin = std::make_shared<TexturedLightedMeshRenderable>(textureShader, penguin_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/penguin.png");
    penguin->setGlobalTransform(
        getTranslationMatrix(0.0f, 1.0f, 1.0f) *
        getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) *
        getScaleMatrix(0.5f)
    );
    // Create Steel Driver (penguin kart)
    const std::string steel_driver_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/steel_driver.obj";
    steel_driver = std::make_shared<TexturedLightedMeshRenderable>(textureShader, steel_driver_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/steel_driver.png");

    steel_driver->setGlobalTransform(
        getTranslationMatrix(30.0f, 1.3f, -0.6f) *
        getRotationMatrix(M_PI_2f, 0.0f, -1.0f, 0.0f) *
        getScaleMatrix(0.06f)
    );
    viewer.addRenderable(steel_driver);
    HierarchicalRenderable::addChild(steel_driver,
                                     penguin);

    /*
    // Create Pianta
    const std::string pianta_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/pianta.obj";
    TexturedLightedMeshRenderablePtr pianta = std::make_shared<TexturedLightedMeshRenderable>(textureShader, pianta_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/pianta.png");
    pianta->setGlobalTransform(
        getTranslationMatrix(0.0f, 5.0f, -4.0f) *
        getRotationMatrix(M_PI_2f, -1.0f, 0.0f, 0.0f) *
        getScaleMatrix(0.3f));

    // Create Tri Speeder (pianta kart)
    const std::string tri_speeder_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/tri_speeder.obj";
    TexturedLightedMeshRenderablePtr tri_speeder = std::make_shared<TexturedLightedMeshRenderable>(textureShader, tri_speeder_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/tri_speeder.png");
    tri_speeder->setGlobalTransform(
        getTranslationMatrix(30.0f, 1.3f, -2.6f) *
        getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) *
        getRotationMatrix(M_PI_2f, 0.0f, 0.0f, 1.0f) *
        getScaleMatrix(0.06f)
    );
    viewer.addRenderable(tri_speeder);
    HierarchicalRenderable::addChild(tri_speeder,
                                     pianta);
    */

    //---------------------------------Placement des autres objets------------------------------------

    // Create Bob-Omb
    const std::string bobOmb_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/bob-omb.obj";

    auto bobOmb = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bobOmb_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/bob-omb.png");

    bobOmb->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -5.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(bobOmb);

    // Create green and red shells
    const std::string shell_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/shell.obj";

    auto green_shell = std::make_shared<TexturedLightedMeshRenderable>(textureShader, shell_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/green_shell.png");
    auto red_shell = std::make_shared<TexturedLightedMeshRenderable>(textureShader, shell_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/red_shell.png");

    green_shell->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -4.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getRotationMatrix(M_PI_2f, 0.0f, 0.0f, 1.0f) * getScaleMatrix(0.1f));
    red_shell->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -6.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getRotationMatrix(M_PI_2f, 0.0f, 0.0f, 1.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(green_shell);
    viewer.addRenderable(red_shell);

    // Create Lightning
    const std::string lightning_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/lightning.obj";

    auto lightning = std::make_shared<TexturedLightedMeshRenderable>(textureShader, lightning_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/lightning.png");

    lightning->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -3.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    // Not used in the scene
    // viewer.addRenderable(lightning);

    // Create Mushroom
    const std::string mushroom_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/mushroom.obj";

    auto mushroom = std::make_shared<TexturedLightedMeshRenderable>(textureShader, mushroom_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/mushroom.png");

    mushroom->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -7.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(mushroom);

    // Create Blue Shell
    const std::string blueShell_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/blue-shell.obj";

    auto blueShell = std::make_shared<TexturedLightedMeshRenderable>(textureShader, blueShell_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/blue-shell.png");

    blueShell->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -2.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    // Not used in the scene
    // viewer.addRenderable(blueShell);

    // Create Star Trophy
    const std::string stars_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/trophy/stars.obj";
    const std::string matMetal_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/trophy/matMetal.obj";
    const std::string metal_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/trophy/metal.obj";
    const std::string woodPanel_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/trophy/woodPanel.obj";

    auto stars = std::make_shared<TexturedLightedMeshRenderable>(textureShader, stars_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/trophy/stars.png");
    auto matMetal = std::make_shared<TexturedLightedMeshRenderable>(textureShader, matMetal_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/trophy/matMetal.png");
    auto metal = std::make_shared<TexturedLightedMeshRenderable>(textureShader, metal_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/trophy/metal.png");
    auto woodPanel = std::make_shared<TexturedLightedMeshRenderable>(textureShader, woodPanel_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/trophy/woodPanel.png");

    stars->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, -1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.8f));
    matMetal->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, -1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.8f));
    metal->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, -1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.8f));
    woodPanel->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, -1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.8f));

    // Not used in the scene
    /*
    viewer.addRenderable(stars);
    viewer.addRenderable(matMetal);
    viewer.addRenderable(metal);
    viewer.addRenderable(woodPanel);
    */

    // Create Star
    const std::string star_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/star.obj";

    auto star = std::make_shared<TexturedLightedMeshRenderable>(textureShader, star_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/star.png");

    star->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, 0.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.2f));

    // Not used in the scene
    // viewer.addRenderable(star);

    // Create Thwomp
    const std::string thwomp_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/thwomp.obj";

    auto thwomp1 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, thwomp_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/thwomp3.png");
    auto thwomp2 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, thwomp_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/thwomp3.png");

    const float thwomp_scale = 0.01f;
    thwomp1->setGlobalTransform(getTranslationMatrix(47.7f, 1.2f, -15.0f) * getScaleMatrix(thwomp_scale));
    thwomp2->setGlobalTransform(getTranslationMatrix(44.4f, 1.2f, -15.0f) * getScaleMatrix(thwomp_scale));

    thwomp_animation(thwomp1);
    thwomp_animation(thwomp2);

    viewer.addRenderable(thwomp1);
    viewer.addRenderable(thwomp2);

    // Create Mystery Cube
    const std::string mystery_cube_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/mystery_cube.obj";

    auto mystery_cube_1 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, mystery_cube_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/mystery_cube.jpg");
    auto mystery_cube_2 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, mystery_cube_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/mystery_cube.jpg");

    mystery_cube_1->setGlobalTransform(getTranslationMatrix(1.9f, 1.4f, -20.0f) * getRotationMatrix(glm::radians(90.0f), 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.02f));
    mystery_cube_2->setGlobalTransform(getTranslationMatrix(46.0f, 1.5f, -30.0f) * getScaleMatrix(0.02f));

    viewer.addRenderable(mystery_cube_1);
    viewer.addRenderable(mystery_cube_2);

    // Create Rainbow Road
    const std::string rainbow_path = "../../sfmlGraphicsPipeline/meshes/rainbow_road.obj";
    const std::string rainbow_texture_path = "../../sfmlGraphicsPipeline/textures/rainbow_road.png";
    auto rainbow = std::make_shared<TexturedLightedMeshRenderable>(
        textureShader,
        rainbow_path,
        myMaterial,
        rainbow_texture_path
    );
    rainbow->setGlobalTransform(getScaleMatrix(0.4f));

    viewer.addRenderable(rainbow);

    // Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    // Create a ground plane, so that particles can bounce on it
    // To adapat with the point the bob-omb will explode
    glm::vec3 p1(40.0f, 1.9f, -47.0f);
    glm::vec3 p2(30.0f, 1.9f, -47.0f);
    glm::vec3 p3(30.0f, 1.9f, -44.0f);
    glm::vec3 p4(40.0f, 1.9f, -44.0f);

    PlanePtr groundQuad = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(groundQuad);

    // Create a renderable associated to the dynamic system
    // This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    // It is also responsible for some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    /*
    ANIMATIONS START HERE
    */

    auto kartRenderable = kart->getRenderable();
    kartBowser_animation(kartRenderable);
    kartPenguin_animation(viewer, steel_driver);

    movingBobomb(viewer, bobOmb);

    auto bobOmbExplosion = std::make_shared<BobOmbExplosion>(
        system,
        lap2_start_time + 21.4f,                  // trigger time of the explosion
        lap2_start_time + 21.4f + 5.0f,           // trigger time of deletion of particles
        bobOmb,
        systemRenderable,
        &viewer                                   // pass pointer to viewer
    );
    viewer.addRenderable(bobOmbExplosion);
}

int main() {
    glm::vec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
    Viewer viewer(1280, 720, background_color);

    initialize_scene(viewer);

    glCullFace(GL_BACK);

    auto camera = viewer.getCamera();
    // float camera_animation_timer = camera_intro_animation(camera);

    bool camera_follow_kart = false;
    bool kart_wheel_rotating = false;

    viewer.startAnimation();

    while(viewer.isRunning()) {
        viewer.handleEvent();

        //printf("%f, %f, %f \n", camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);

        const float time = viewer.getTime();

        if (time <= 12.0) {
            leftFire->setMaterial(offMat);
            middleFire->setMaterial(offMat);
            rightFire->setMaterial(offMat);
        }else if (time <= 13.0) {
            leftFire->setMaterial(redMat);
        }else if (time <= 14.0) {
            middleFire->setMaterial(redMat);
        }else if (time <= 15.0) {
            rightFire->setMaterial(redMat);
        }else if (time <= 15.8) {
            leftFire->setMaterial(greenMat);
            middleFire->setMaterial(greenMat);
            rightFire->setMaterial(greenMat);
        }

        if (!camera_follow_kart && camera_animation_timer - time <= 0.0f) {
            camera_animation_follow_kart(camera, camera_animation_timer);
            camera_follow_kart = true;
        }
        if (!kart_wheel_rotating && time >= RACE_START_TIME - 2.0f) {
            kart->startRotateWheels();
            kart_wheel_rotating = true;
        }

        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//----------------------------------Animation du Bowser-------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
void kartBowser_animation(std::shared_ptr<SkeletonRenderable> &kart_root) {
    float epsilon = 0.0001f;

    float animation_time = 0.0f;
    const float scale = 0.05f;

    // 1st straight line
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -1.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // Don't move until the race starts.
    animation_time += 15.0f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -1.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {12.0f, 1.17f, -1.4f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.0f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // first right turn

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {5.5f, 1.17f, -2.8f},
            glm::angleAxis(glm::radians(230.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.8f, 1.17f, -4.5f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.0f, 1.17f, -5.9f},
            glm::angleAxis(glm::radians(190.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.17f, -8.4f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.17f, -10.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second straight line

    animation_time += 1.0f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.8f, 1.17f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.8f, 1.17f, -30.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second left turn

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.0f, 1.17f, -32.0f},
            glm::angleAxis(glm::radians(160.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.5f, 1.17f, -34.0f},
            glm::angleAxis(glm::radians(140.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {5.0f, 1.0f, -35.5f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.0f, 0.85f, -36.0f},
            glm::angleAxis(glm::radians(100.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 0.8f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third straight line

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {11.0f, 0.85f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {13.0f, 0.75f, -37.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.65f, -38.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third right turn (Bowser falls off the track here)

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -1.0f, -38.0f},
            glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -3.0f, -38.0f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -6.0f, -38.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );


    animation_time += 1.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -120.0f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // reset position back on track

    // Letting some time for Lakitu to put the kart back on the track

    animation_time += 3.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    animation_time += 10e-6;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -32.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -30.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {19.5f, 0.4f, -20.0f},
            glm::angleAxis(glm::radians(10.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );


    // left turn

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {19.0f, 0.4f, -19.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -16.5f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.5f, 0.4f, -15.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {24.0f, 0.4f, -14.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {25.5, 0.4f, -15.5f},
            glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // Drift into ramp

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.3f, 0.4f, -17.0f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.0f, 1.0f, -19.0f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.0f, 1.2f, -20.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.0f, 1.0f, -21.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // landing after the ramp

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.0f, 0.65f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.73f, -25.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.8f, -26.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.9f, -27.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.0f, -29.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.5f, -39.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 1.8f, -44.0f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.9f, -46.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {34.0f, 1.9f, -46.5f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.0f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {36.5f, 1.9f, -47.5f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {40.0f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.6f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.5f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.3f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.5f, 1.8f, -46.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.7f, -45.0f},
            glm::angleAxis(glm::radians(50.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.65f, -43.0f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.4f, -41.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    /*
     TODO MUSHROOM THERE
    */

    // straight line until ramp

    animation_time += 1.0f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -30.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.58f, -21.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // TODO ADD A BANANA AFTER BOWSER JUMPS
    // ramp jump

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 3.5f, -16.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -10.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // landing after ramp

    animation_time += 0.4f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.1f, 1.17f, -6.0f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.17f, -4.0f},
            glm::angleAxis(glm::radians(330.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.17f, -3.0f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.0f, 1.17f, -2.5f},
            glm::angleAxis(glm::radians(285.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line back to the start

    animation_time += 1.9f;
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -2.0f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    /*/
    2nd LAP
    */

    lap2_start_time = animation_time;

    // MUSHROOM BOOST AT THE START OF THE LAP

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {12.5f, 1.17f, -1.9f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 0.7f
    );

    // First right turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {6.0f, 1.17f, -2.5f},
            glm::angleAxis(glm::radians(235.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 1.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.5f, 1.17f, -6.0f},
            glm::angleAxis(glm::radians(195.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 1.8f
    );

    // second straight line

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {1.0f, 1.17f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 2.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {1.0f, 1.17f, -28.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 3.7f
    );

    // second right turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.5f, 1.17f, -34.0f},
            glm::angleAxis(glm::radians(140.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {6.0f, 0.85f, -36.0f},
            glm::angleAxis(glm::radians(100.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 0.8f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.8f
    );

    // third straight line

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {13.0f, 0.75f, -37.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 5.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.65f, -38.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 5.6f
    );

    // third left turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -35.5f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -33.5f},
            glm::angleAxis(glm::radians(40.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.5f, -30.0f},
            glm::angleAxis(glm::radians(20.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.6f
    );

    // straight line (little bump)

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.6f, -28.5f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.8f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.8f, -27.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.4f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -20.0f},
            glm::angleAxis(glm::radians(10.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.7f
    );

    // left turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.9f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -16.5f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.5f, 0.4f, -15.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {24.5f, 0.4f, -14.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.8f
    );

    // TODO INSERT RED SHELL HIT ANIMATION HERE
    // red shell hit Bowser here, making him stop + spin for a bit

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {26.5f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.2f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.1f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.8f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.4f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.3f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.1f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(240.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.8f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.5f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.4f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.9f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.7f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.5f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.8f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.9f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 10.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 11.0f
    );


    // left turn (static)

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 14.0f
    );

     // straight line after the red shell hit, low speed

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.5f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 16.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.5f, 1.8f, -43.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 20.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.5f, 1.9f, -45.0f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 20.2f
    );

    // right turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.5f, 1.9f, -46.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 20.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.0f, 1.9f, -46.2f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.0f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.5f
    );

    // TODO ADD BOB-OMB HIT ANIMATION HERE
    // straight line + bob-omb

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {38.0f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.7f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {38.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.8f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {39.0f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.9f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {39.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {40.0f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(240.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.1f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {40.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {41.0f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.3f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {41.6f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.1f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.5f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.6f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {43.1f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.7f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {43.6f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.8f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.1f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 22.9f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.9f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.1f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.2f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(190.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.3f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.7f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(230.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.9f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 23.5f
    );

    // stop time
    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.9f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 24.5f
    );

    // turn left (static)

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.9f, 1.9f, -46.8f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 26.0f
    );

    // straight line

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.8f, 1.3f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 29.5f
    );

    // Do not go on ramp 

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {47.3f, 1.3f, -23.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 29.8f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {47.3f, 1.3f, -15.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 31.1f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {47.1f, 1.3f, -11.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 31.5f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 31.8f
    );


    // right turn

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.9f, 1.17f, -6.0f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 33.0f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.4f, 1.17f, -4.0f},
            glm::angleAxis(glm::radians(330.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 33.2f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {43.9f, 1.17f, -3.0f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 33.4f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.0f, 1.17f, -1.8f},
            glm::angleAxis(glm::radians(285.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 33.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {39.9f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 33.9f
    );

    // straight line (a billball stamps on Bowser)

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.5f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 34.5f
    );

    //  TODO INSERT BILLBALL HERE

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.4f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 34.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 41.6f
    );

    kart_root->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 50.0f
    );

    auto car = kart->get_car();
    car->addGlobalTransformKeyframe(
        GeometricTransformation(
            glm::vec3(0.0f),
            qY(0.0f),
            glm::vec3(1.0f)
        ),
        0.0f
    );
    car->addGlobalTransformKeyframe(
        GeometricTransformation(
            glm::vec3(0.0f),
            qY(0.0f),
            glm::vec3(1.0f)
        ),
        lap2_start_time + 37.6f
    );
    car->addGlobalTransformKeyframe(
        GeometricTransformation(
            glm::vec3(0.0f),
            qY(0.0f),
            glm::vec3(1.0f, 0.2f, 1.0f)
        ),
        lap2_start_time + 37.6f + 10e-6
    );
    car->addGlobalTransformKeyframe(
        GeometricTransformation(
            glm::vec3(0.0f),
            qY(0.0f),
            glm::vec3(1.0f, 0.2f, 1.0f)
        ),
        lap2_start_time + 47.0f
    );

    // Finish line yayyy

    // endless animation to not loop

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 1.17, -1.6f},   
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        lap2_start_time + 47.00000001f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, -1000.0f, -1.6f},   
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        lap2_start_time + 1000.0f
    );

}

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//----------------------------------Animation du bob-omb------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void movingBobomb(Viewer& viewer, TexturedLightedMeshRenderablePtr& bobOmbRenderable)
{
    const float scale = 0.1f;
    constexpr float epsilonScale = 0.0001f;

    // Hide de bob-omb at the beginning

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.8f, -35.0f},
            qY(-2.2f),
            glm::vec3(epsilonScale)
        ),
        lap2_start_time + 0.0f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.8f, -35.0f},
            qY(-2.2f),
            glm::vec3(epsilonScale)
        ),
        lap2_start_time + 18.39f
    );

    // Bob-omb appears + balistic movement

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.8f, -35.0f},
            qY(-2.2f),
            glm::vec3(scale)
        ),
        lap2_start_time + 18.4f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.5f, 2.8f, -37.8f},
            qY(-2.0f),
            glm::vec3(scale)
        ),
        lap2_start_time + 18.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {34.0f, 4.5f, -40.5f},
            qY(-1.7f),
            glm::vec3(scale)
        ),
        lap2_start_time + 18.8f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {34.6f, 5.9f, -43.2f},
            qY(-1.4f),
            glm::vec3(scale)
        ),
        lap2_start_time + 19.0f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.8f, 5.6f, -45.0f},
            qY(-1.1f),
            glm::vec3(scale)
        ),
        lap2_start_time + 19.2f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {36.6f, 4.5f, -46.0f},
            qY(-0.8f),
            glm::vec3(scale)
        ),
        lap2_start_time + 19.4f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.2f, 3.05f, -46.6f},
            qY(-0.4f),
            glm::vec3(scale)
        ),
        lap2_start_time + 19.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.5f, 2.35f, -46.8f},
            qY(0.0f),
            glm::vec3(scale)
        ),
        lap2_start_time + 19.8f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.5f, 2.3, -46.8f},
            qY(0.0f),
            glm::vec3(scale)
        ),
        lap2_start_time + 20.0f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.5f, 2.3f, -46.8f},
            qY(0.0f),
            glm::vec3(scale)
        ),
        lap2_start_time + 21.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.5f, 2.3f, -46.8f},
            qY(5.0f),
            glm::vec3(epsilonScale) // Scales down to disappear
        ),
        lap2_start_time + 21.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, -1000.0f, 0.0f}, // Push the bob-omb far below scene
            qY(5.0f),
            glm::vec3(epsilonScale)
        ),
        200.0f // Freeze the animation, our film won't exceed 200 seconds
    );
}




//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//----------------------------------Animation du pingouin-----------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void kartPenguin_animation(Viewer& viewer, TexturedLightedMeshRenderablePtr& steel_driver) {
    const float scale = 0.06f;
    float epsilon = 0.0001f;

    float animation_time = 0.0f;

    // 1st straight line
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.0f, 1.3f, -0.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // Don't move until the race starts.
    animation_time += 15.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.0f, 1.3f, -0.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {15.0f, 1.3f, -1.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 1.3f, -0.6f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // first right turn

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.5f, 1.3f, -1.0f},
            glm::angleAxis(glm::radians(230.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.8f, 1.3f, -2.5f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.0f, 1.3f, -4.0f},
            glm::angleAxis(glm::radians(190.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.3f, -6.4f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.3f, -8.5f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second straight line

    animation_time += 1.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.0f, 1.3f, -18.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.0f, 1.3f, -25.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.0f, 1.3f, -30.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second left turn

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.0f, 1.3f, -32.0f},
            glm::angleAxis(glm::radians(160.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.5f, 1.3f, -34.0f},
            glm::angleAxis(glm::radians(140.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {6.0f, 1.1f, -35.0f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 0.9f, -35.5f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third straight line

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {12.0f, 0.8f, -35.5f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.9f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.85f, -36.5f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.7f, -38.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.6f, -35.0f},
            glm::angleAxis(glm::radians(40.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.6f, -29.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 1.3f, -24.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 0.6f, -20.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // left turn

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 0.4f, -19.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.8f, 0.4f, -18.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.8f, 0.4f, -18.0f},
            glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.2f, 0.4f, -17.5f},
            glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.2f, 0.4f, -16.5f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {24.0f, 0.4f, -16.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line into ramp (left)

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {26.0f, 0.4f, -15.7f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.0f, 1.0f, -18.0f},
            glm::angleAxis(glm::radians(160.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // fails ramp

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.7f, 1.2f, -20.0f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.4f, 1.0f, -21.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.9f, 0.65f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.9f, 0.35f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.9f, -6.0f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 3.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.9f, -120.0f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // reset position back on track

    // Letting some time for Lakitu to put the kart back on the track

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.78f, -22.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    animation_time += 0.001f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.9f, -22.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.9f, -22.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 2.1f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.9f, -37.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.9f, -44.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // stops him before teleport him

    animation_time += 3.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.9f, -44.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );


    // Teleport him right behind Bowser

    animation_time += 0.001f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -8.0f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.1f, 1.3f, -6.0f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.3f, -4.0f},
            glm::angleAxis(glm::radians(330.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.3f, -3.0f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.0f, 1.3f, -1.6f},
            glm::angleAxis(glm::radians(285.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {38.0f, 1.3f, -1.2f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // TODO MUSHROOM HERE
    // straight line, mushroom

    animation_time += 1.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.5f, 1.3f, -0.7f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    /*
    2nd LAP
    */

    animation_time += 1.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {10.5f, 1.3f, -0.7f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // first right turn 

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {4.0f, 1.3f, -1.5f},
            glm::angleAxis(glm::radians(242.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.5f, 1.3f, -4.0f},
            glm::angleAxis(glm::radians(199.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {1.5f, 1.3f, -8.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second straight line

    animation_time += 1.1f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.7f, 1.3f, -22.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.7f, 1.3f, -32.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.7f, 1.3f, -34.0f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.0f, 1.3f, -36.0f},
            glm::angleAxis(glm::radians(105.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.2f, 1.3f, -36.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third straight line

    animation_time += 0.8f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.9f, -34.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third right turn

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {19.5f, 0.6f, -36.0f},
            glm::angleAxis(glm::radians(40.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.6f, -33.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.6f, -27.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // staight line, make him slow down (Bowser sees it)

    animation_time += 2.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.6f, -20.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // TODO RED SHELL ANIMATION HERE
    // left turn, launch a red shell towards Bowser

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.6f, -18.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 0.6f, -17.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.5f, 0.6f, -17.0f},
            glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.9f, 0.6f, -16.5f},
            glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.9f, 0.6f, -15.5f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line 

    animation_time += 1.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.0f, 0.6f, -15.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // left turn 

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.0f, 0.6, -17.0f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.5f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(160.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // stops him midturn, quick 360 

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(240.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(300.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(330.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -19.5f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line 

    animation_time += 3.5f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 2.1f, -45.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn 

    animation_time += 0.6f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.5f, 2.1f, -46.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.6f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.0f, 2.1f, -46.2f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {37.0f, 2.1f, -46.8f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );
    
 // straight line

    animation_time += 0.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {40.0f, 2.1f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.5f, 2.1f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.5f, 2.0f, -46.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.9f, -45.0f},
            glm::angleAxis(glm::radians(50.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.85f, -43.0f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.6f, -41.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line 

    animation_time += 1.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.5f, -30.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.5f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // go right at the ramp

    animation_time += 0.7f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -23.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 2.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -10.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // stops there for a moment

    animation_time += 1.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -10.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // goes back

    animation_time += 1.4f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -18.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // stops again

    animation_time += 2.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -18.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // restart going forward

    animation_time += 1.2f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.5f, -15.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.4f, 1.5f, -11.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.5f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // scales down the model to disappear

    animation_time += 0.0001f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.5f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // make him goes far away

    animation_time += 0.0001f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, -10000.0f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // endless animation 

    animation_time += 1000.0f;
    steel_driver->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, -10000.0f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // transforms himself in Bill Ball
}

#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <texturing/TexturedMeshRenderable.hpp>
#include <Viewer.hpp>
#include <lighting/LightedMeshRenderable.hpp>
#include <lighting/LightedCubeRenderable.hpp>
#include <lighting/LightedCylinderRenderable.hpp>
#include <lighting/DirectionalLightRenderable.hpp>
#include <lighting/PointLightRenderable.hpp>
#include <lighting/SpotLightRenderable.hpp>
#include <FrameRenderable.hpp>
#include <lighting/Material.hpp>
#include <texturing/CubeMapRenderable.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <Utils.hpp>

#include "bowser.hpp"
#include "kart.hpp"

#define RACE_START_TIME 15.0f  // seconds

KartPtr kart;

void kartBowser_animation(Viewer& viewer, TexturedLightedMeshRenderablePtr& kart);

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
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(5.0,5.0,5.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    directionalLight->setGlobalTransform(getTranslationMatrix(lightPosition) * directionalLight->getGlobalTransform());

    viewer.addDirectionalLight(directionalLight);

    //Lumières de spots
    glm::vec3 s_position(0.0,5.0,-5.0), s_spotDirection = glm::normalize(glm::vec3(0.0,-1.0,1.0));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.5,0.5,0.5), s_specular(0.5,0.5,0.5);
    float s_constant=1.0, s_linear=0.0, s_quadratic=0.0;
    float s_innerCutOff=std::cos(glm::radians(10.0f)), s_outerCutOff=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight1 = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_innerCutOff, s_outerCutOff);

    glm::vec3 s_position2(2.5,5,2.5), s_spotDirection2 = glm::normalize(glm::vec3(-0.5,-1.0,-0.5));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient2(0.0,0.0,0.0), s_diffuse2(0.5,0.5,0.5), s_specular2(0.5,0.5,0.5);
    float s_constant2=1.0, s_linear2=0.0, s_quadratic2=0.0;
    float s_innerCutOff2=std::cos(glm::radians(10.0f)), s_outerCutOff2=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight2 = std::make_shared<SpotLight>(s_position2, s_spotDirection2,
                                                         s_ambient2, s_diffuse2, s_specular2,
                                                         s_constant2, s_linear2, s_quadratic2,
                                                         s_innerCutOff2, s_outerCutOff2);

    glm::vec3 s_position3(-2.5,5.0,2.5), s_spotDirection3 = glm::normalize(glm::vec3(0.5,-1.0,-0.5));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient3(0.0,0.0,0.0), s_diffuse3(0.5,0.5,0.5), s_specular3(0.5,0.5,0.5);
    float s_constant3=1.0, s_linear3=0.0, s_quadratic3=0.0;
    float s_innerCutOff3=std::cos(glm::radians(10.0f)), s_outerCutOff3=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight3 = std::make_shared<SpotLight>(s_position3, s_spotDirection3,
                                                         s_ambient3, s_diffuse3, s_specular3,
                                                         s_constant3, s_linear3, s_quadratic3,
                                                         s_innerCutOff3, s_outerCutOff3);

    viewer.addSpotLight(spotLight1);
    viewer.addSpotLight(spotLight2);
    viewer.addSpotLight(spotLight3);

    //Ajout d'un matériel pour nos objets 3D
    glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
    float mShininess=0.0;
    // MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
    MaterialPtr myMaterial = Material::Bronze();

    BowserPtr bowser = std::make_shared<Bowser>(textureShader, myMaterial,
                                                glm::vec3(0.0f, 4.0f, 0.0f),
                                                0.7f);
    // Create Kart
    kart = std::make_shared<Kart>(textureShader, myMaterial, 0.05f);
    kart->getRenderable()->setGlobalTransform(
        getTranslationMatrix(28.5f, 1.17f, -1.6f) *
        getRotationMatrix(-M_PI_2f, 0.0f, 1.0f, 0.0f) *
        getScaleMatrix(0.05f)
    );
    viewer.addRenderable(kart->getRenderable());
    HierarchicalRenderable::addChild(kart->getRenderable(),
                                     bowser->getRenderable());

    // Create Lakitu
    const std::string lakitu_path = "../../sfmlGraphicsPipeline/meshes/lakitu.obj";

    auto lakitu = std::make_shared<TexturedLightedMeshRenderable>(textureShader, lakitu_path, myMaterial, "../../sfmlGraphicsPipeline/textures/lakitu.png");

    // lakitu->setGlobalTransform(getTranslationMatrix(0.35f, 2.0f, -1.0f) * getScaleMatrix(0.02f));

    lakitu->setGlobalTransform(getTranslationMatrix(23.0f, 3.5f, 2.0f) * getScaleMatrix(0.02f) * getRotationMatrix(M_PI*120/180, {0.0f, -1.0f, 0.0f}));
    viewer.addRenderable(lakitu);

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

    viewer.addRenderable(lightning);

    // Create Mushroom
    const std::string mushroom_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/mushroom.obj";

    auto mushroom = std::make_shared<TexturedLightedMeshRenderable>(textureShader, mushroom_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/mushroom.png");

    mushroom->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -7.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(mushroom);

    // Create Penguin
    const std::string penguin_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/penguin.obj";

    auto penguin = std::make_shared<TexturedLightedMeshRenderable>(textureShader, penguin_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/penguin.png");

    penguin->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, -8.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(penguin);

    // Create Pianta
    const std::string pianta_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/pianta.obj";

    auto pianta = std::make_shared<TexturedLightedMeshRenderable>(textureShader, pianta_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/pianta.png");

    pianta->setGlobalTransform(getTranslationMatrix(-4.5f, 0.0f, -10.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(pianta);


    // Create Blue Shell
    const std::string blueShell_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/blue-shell.obj";

    auto blueShell = std::make_shared<TexturedLightedMeshRenderable>(textureShader, blueShell_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/blue-shell.png");

    blueShell->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, -2.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(blueShell);

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

    viewer.addRenderable(stars);
    viewer.addRenderable(matMetal);
    viewer.addRenderable(metal);
    viewer.addRenderable(woodPanel);

    // Create Star
    const std::string star_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/star.obj";

    auto star = std::make_shared<TexturedLightedMeshRenderable>(textureShader, star_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/star.png");

    star->setGlobalTransform(getTranslationMatrix(-5.0f, 0.0f, 0.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getRotationMatrix(M_PI_2f, 1.0f, 0.0f, 0.0f) * getScaleMatrix(0.2f));

    viewer.addRenderable(star);

    // Create Thwomp
    const std::string thwomp_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/thwomp.obj";

    auto thwomp1 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, thwomp_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/thwomp1.png");
    auto thwomp2 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, thwomp_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/thwomp2.png");
    auto thwomp3 = std::make_shared<TexturedLightedMeshRenderable>(textureShader, thwomp_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/thwomp3.png");

    thwomp1->setGlobalTransform(getTranslationMatrix(-5.0f, 5.0f, 1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.01f));
    thwomp2->setGlobalTransform(getTranslationMatrix(-5.0f, 2.5f, 1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.01f));
    thwomp3->setGlobalTransform(getTranslationMatrix(-5.0f, -0.5f, 1.0f) * getRotationMatrix(M_PI_2f, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.01f));

    viewer.addRenderable(thwomp1);
    viewer.addRenderable(thwomp2);
    viewer.addRenderable(thwomp3);

    // Create Rainbow Road
    const std::string rainbow_path = "../../sfmlGraphicsPipeline/meshes/rainbow_road.obj";
    const std::string rainbow_texture_path = "../../sfmlGraphicsPipeline/textures/rainbow_road.png";
    auto rainbow = std::make_shared<TexturedMeshRenderable>(
        texShader,
        rainbow_path,
        rainbow_texture_path
    );
    rainbow->setGlobalTransform(getScaleMatrix(0.4f));

    viewer.addRenderable(rainbow);

    /*
    ANIMATIONS START HERE
    */

    auto kartRenderable = kart->getRenderable();
    kartBowser_animation(viewer, kartRenderable);
}

int main() {
    glm::vec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
    Viewer viewer(1280, 720, background_color);
    initialize_scene(viewer);

    glCullFace(GL_BACK);

    auto camera = viewer.getCamera();
    float camera_animation_timer = camera_intro_animation(camera);

    bool camera_follow_kart = false;
    bool kart_wheel_rotating = false;

    viewer.startAnimation();
    while(viewer.isRunning()) {
        viewer.handleEvent();

        const float time = viewer.getTime();
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

void kartBowser_animation(Viewer& viewer, TexturedLightedMeshRenderablePtr& kart) {
    const float scale = 0.05f;
    float epsilon = 0.0001f;

    float animation_time = 0.0f;

    // 1st straight line
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -1.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // Don't move until the race starts.
    animation_time += 15.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -1.6f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {12.0f, 1.17f, -1.4f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.8f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.0f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(250.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // first right turn

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {5.5f, 1.17f, -2.8f},
            glm::angleAxis(glm::radians(230.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.8f, 1.17f, -4.5f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.0f, 1.17f, -5.9f},
            glm::angleAxis(glm::radians(190.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.17f, -8.4f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.5f, 1.17f, -10.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second straight line

    animation_time += 1.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.8f, 1.17f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.8f, 1.17f, -30.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // second left turn

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.0f, 1.17f, -32.0f},
            glm::angleAxis(glm::radians(160.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.5f, 1.17f, -34.0f},
            glm::angleAxis(glm::radians(140.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {5.0f, 1.0f, -35.5f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {7.0f, 0.85f, -36.0f},
            glm::angleAxis(glm::radians(100.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 0.8f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third straight line

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {11.0f, 0.85f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {13.0f, 0.75f, -37.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.65f, -38.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // third left turn (Bowser falls off the track here)

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(60.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -1.0f, -38.0f},
            glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -3.0f, -38.0f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -6.0f, -38.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );


    animation_time += 1.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.0f, -120.0f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    // reset position back on track

    // Letting some time for Lakitu to put the kart back on the track

    animation_time += 6.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(epsilon)
        ),
        animation_time
    );

    animation_time += 10e-6;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -38.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -32.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -30.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {19.5f, 0.4f, -20.0f},
            glm::angleAxis(glm::radians(10.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );


    // left turn

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {19.0f, 0.4f, -19.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -16.5f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.5f, 0.4f, -15.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {24.0f, 0.4f, -14.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {25.5, 0.4f, -15.5f},
            glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // Drift into ramp

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.3f, 0.4f, -17.0f},
            glm::angleAxis(glm::radians(120.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.0f, 1.0f, -19.0f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.0f, 1.2f, -20.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.0f, 1.0f, -21.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // landing after the ramp

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.0f, 0.65f, -23.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.73f, -25.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.8f, -26.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 0.9f, -27.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.0f, -29.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 1.0f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.5f, -39.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 1.8f, -44.0f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {33.0f, 1.9f, -46.0f},
            glm::angleAxis(glm::radians(150.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {34.0f, 1.9f, -46.5f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {35.0f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(110.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {36.5f, 1.9f, -47.5f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {40.0f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.6f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.5f, 1.9f, -47.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.3f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.5f, 1.8f, -46.0f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.7f, -45.0f},
            glm::angleAxis(glm::radians(50.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.3f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.65f, -43.0f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
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
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -30.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.5f;
    kart->addGlobalTransformKeyframe(
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
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 3.5f, -16.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -10.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // landing after ramp

    animation_time += 0.4f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.0f, 1.3f, -8.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // right turn

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {46.1f, 1.17f, -6.0f},
            glm::angleAxis(glm::radians(-180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {45.5f, 1.17f, -4.0f},
            glm::angleAxis(glm::radians(-160.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {44.0f, 1.17f, -3.0f},
            glm::angleAxis(glm::radians(-140.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    animation_time += 0.2f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {42.0f, 1.17f, -1.8f},
            glm::angleAxis(glm::radians(-110.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    // straight line back to the start

    animation_time += 1.9f;
    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.5f, 1.17f, -1.6f},
            glm::angleAxis(glm::radians(-90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        animation_time
    );

    /*/
    2nd ROUND
    */

    float lap2_start_time = animation_time;


    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {12.5f, 1.17f, -1.45f},
            glm::angleAxis(-M_PI_2f, glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 0.7f
    );

    // First right turn

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {6.0f, 1.17f, -2.5f},
            glm::angleAxis(glm::radians(235.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 1.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {2.5f, 1.17f, -6.0f},
            glm::angleAxis(glm::radians(195.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 1.8f
    );

    // second straight line

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {1.0f, 1.17f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 2.6f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {1.0f, 1.17f, -28.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 3.7f
    );

    // second left turn

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {3.5f, 1.17f, -34.0f},
            glm::angleAxis(glm::radians(140.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {6.0f, 0.85f, -36.0f},
            glm::angleAxis(glm::radians(100.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.6f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {9.0f, 0.8f, -36.2f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 4.8f
    );

    // third straight line

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {13.0f, 0.75f, -37.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 5.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {18.0f, 0.65f, -38.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 5.6f
    );

    // third left turn

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -35.5f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -33.5f},
            glm::angleAxis(glm::radians(40.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.4f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.5f, -30.0f},
            glm::angleAxis(glm::radians(20.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.6f
    );

    // straight line (little bump)

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.6f, -28.5f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 6.8f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.8f, -27.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.0f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.5f, 0.4f, -25.0f},
            glm::angleAxis(glm::radians(0.0f), glm::vec3(0,1,0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -20.0f},
            glm::angleAxis(glm::radians(10.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.7f
    );

    // left turn

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.0f},
            glm::angleAxis(glm::radians(15.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 7.9f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {20.0f, 0.4f, -17.5f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {21.0f, 0.4f, -16.5f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.4f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {22.5f, 0.4f, -15.0f},
            glm::angleAxis(glm::radians(80.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.6f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {24.5f, 0.4f, -14.0f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 8.8f
    );

    // TODO INSERT RED SHELL HIT ANIMATION HERE
    // red shell hit Bowser here, making him stop + spin for a bit

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {26.5f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.0f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.2f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(130.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.1f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {27.8f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(170.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.2f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {28.4f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(210.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.3f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.1f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(240.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.4f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {29.8f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(270.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.5f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.4f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(310.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.6f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {30.9f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(350.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.7f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {31.5f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(30.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.8f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(70.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 9.9f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 10.0f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 11.0f
    );


    // left turn (static)

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.4f, -14.3f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 14.0f
    );

    // straight line after the red shell hit, low speed

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 16.0f
    );

    kart->addGlobalTransformKeyframe(
        GeometricTransformation(
            {32.0f, 0.6f, -20.0f},
            glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 1, 0)),
            glm::vec3(scale)
        ),
        lap2_start_time + 20.0f
    );
}

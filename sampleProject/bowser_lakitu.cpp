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

void initialize_scene(Viewer &viewer) {
    // Create a shader program
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");
    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    ShaderProgramPtr phongShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/phongVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/phongFragment.glsl");
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

    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(1.0,1.0,1.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    directionalLight->setGlobalTransform(getTranslationMatrix(lightPosition) * directionalLight->getGlobalTransform());

    DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight);
    localTransformation = getScaleMatrix(0.5);
    directionalLightRenderable->setLocalTransform(localTransformation);
    viewer.addDirectionalLight(directionalLight);
    // viewer.addRenderable(directionalLightRenderable);


    glm::vec3 s_position(0.0,5.0,-5.0), s_spotDirection = glm::normalize(glm::vec3(0.0,-1.0,1.0));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.5,0.5,0.5), s_specular(0.5,0.5,0.5);
    float s_constant=1.0, s_linear=0.0, s_quadratic=0.0;
    float s_innerCutOff=std::cos(glm::radians(10.0f)), s_outerCutOff=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight1 = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_innerCutOff, s_outerCutOff);
    viewer.addSpotLight(spotLight1);
    // SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight1);
    // localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5,0.5,0.5));
    // spotLightRenderable->setLocalTransform(localTransformation);
    // viewer.addRenderable(spotLightRenderable);

    glm::vec3 s_position2(2.5,5,2.5), s_spotDirection2 = glm::normalize(glm::vec3(-0.5,-1.0,-0.5));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient2(0.0,0.0,0.0), s_diffuse2(0.5,0.5,0.5), s_specular2(0.5,0.5,0.5);
    float s_constant2=1.0, s_linear2=0.0, s_quadratic2=0.0;
    float s_innerCutOff2=std::cos(glm::radians(10.0f)), s_outerCutOff2=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight2 = std::make_shared<SpotLight>(s_position2, s_spotDirection2,
                                                         s_ambient2, s_diffuse2, s_specular2,
                                                         s_constant2, s_linear2, s_quadratic2,
                                                         s_innerCutOff2, s_outerCutOff2);
    viewer.addSpotLight(spotLight2);

    glm::vec3 s_position3(-2.5,5.0,2.5), s_spotDirection3 = glm::normalize(glm::vec3(0.5,-1.0,-0.5));
    //glm::vec3 s_ambient(0.0,0.0,0.0), s_diffuse(0.0,0.0,0.0), s_specular(0.0,0.0,0.0);
    glm::vec3 s_ambient3(0.0,0.0,0.0), s_diffuse3(0.5,0.5,0.5), s_specular3(0.5,0.5,0.5);
    float s_constant3=1.0, s_linear3=0.0, s_quadratic3=0.0;
    float s_innerCutOff3=std::cos(glm::radians(10.0f)), s_outerCutOff3=std::cos(glm::radians(20.0f));
    SpotLightPtr spotLight3 = std::make_shared<SpotLight>(s_position3, s_spotDirection3,
                                                         s_ambient3, s_diffuse3, s_specular3,
                                                         s_constant3, s_linear3, s_quadratic3,
                                                         s_innerCutOff3, s_outerCutOff3);
    viewer.addSpotLight(spotLight3);

    //Ajout d'un matériel pour nos objets 3D
    glm::vec3 mAmbient(0.0), mDiffuse(0.0), mSpecular(0.0);
    float mShininess=0.0;
    MaterialPtr myMaterial = std::make_shared<Material>(mAmbient, mDiffuse, mSpecular, mShininess);
    MaterialPtr bronze = Material::Bronze();

    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/simpleTextureVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/simpleTextureFragment.glsl"
    );
    viewer.addShaderProgram(texShader);

    // Create bowser
    const std::string bowser_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_complet.obj";
    const std::string bowser_eyes_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_eyes.obj";
    auto bowser = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_path, bronze, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_eyes = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_eyes_path, bronze, "../../sfmlGraphicsPipeline/textures/bowser/bowser_eyes.png");
    bowser->setGlobalTransform(getTranslationMatrix(0.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));
    bowser_eyes->setGlobalTransform(getTranslationMatrix(0.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));
    // Add bowser to the viewer
    viewer.addRenderable(bowser);
    viewer.addRenderable(bowser_eyes);

    const std::string lakitu_path = "../../sfmlGraphicsPipeline/meshes/lakitu.obj";
    LightedMeshRenderablePtr lakitu = std::make_shared<LightedMeshRenderable>(phongShader, lakitu_path, bronze);
    lakitu->setGlobalTransform(getTranslationMatrix(0.35f, 2.0f, -1.0f) * getScaleMatrix(0.1f));
    // Add bowser to the viewer
    viewer.addRenderable(lakitu);

    const std::string rainbow_path = "../../sfmlGraphicsPipeline/meshes/rainbow_road.obj";
    const std::string rainbow_texture_path =
        "../../sfmlGraphicsPipeline/textures/rainbow_road.png";
    auto rainbow = std::make_shared<TexturedMeshRenderable>(
        texShader,
        rainbow_path,
        rainbow_texture_path
    );
    rainbow->setGlobalTransform(getScaleMatrix(0.2f));
    // Add bowser to the viewer
    viewer.addRenderable(rainbow);
}

int main() {
    glm::vec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
    Viewer viewer(1280, 720, background_color);
    initialize_scene(viewer);

    glCullFace(GL_BACK);

    viewer.startAnimation();
    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

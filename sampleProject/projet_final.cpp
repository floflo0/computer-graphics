#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <cmath>
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

    //Position the camera
    // viewer.getCamera().setViewMatrix( glm::lookAt( glm::vec3(0, 0, 0 ), glm::vec3(0, 0, 0), glm::vec3( 0, 1, 0 ) ) );



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



    // Create bowser
    // const std::string bowser_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_complet.obj";
    const std::string bowser_body_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_body.obj";
    const std::string bowser_arm_right_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_arm_right.obj";
    const std::string bowser_arm_left_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_arm_left.obj";
    const std::string bowser_leg_right_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_leg_right.obj";
    const std::string bowser_leg_left_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_leg_left.obj";
    const std::string bowser_tail_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_tail.obj";
    const std::string bowser_eyes_path = "../../sfmlGraphicsPipeline/meshes/bowser/bowser_eyes.obj";

    // auto bowser = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_body = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_body_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_arm_right = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_arm_right_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_arm_left = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_arm_left_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_leg_right = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_leg_right_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_leg_left = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_leg_left_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_tail = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_tail_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_complet.png");
    auto bowser_eyes = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bowser_eyes_path, myMaterial, "../../sfmlGraphicsPipeline/textures/bowser/bowser_eyes.png");



    // bowser->setGlobalTransform(getTranslationMatrix(0.0f, 0.0f, 0.0f) * getScaleMatrix(0.1f));
    glm::mat4 bowser_mat = getTranslationMatrix(20.0f, 1.444f, -1.6f) * getScaleMatrix(0.04f) * getRotationMatrix(M_PI_2f, {0.0f, 1.0f, 0.0f});


    bowser_body->setGlobalTransform(bowser_mat);
    bowser_arm_right->setGlobalTransform(bowser_mat);
    bowser_arm_left->setGlobalTransform(bowser_mat);
    bowser_leg_right->setGlobalTransform(bowser_mat);
    bowser_leg_left->setGlobalTransform(bowser_mat);
    bowser_tail->setGlobalTransform(bowser_mat);
    bowser_eyes->setGlobalTransform(bowser_mat);

    /* Le centre de rotation du bras n'est pas bon
    const glm::mat4 rotatededArm = glm::rotate(
        bowser_arm_right->getGlobalTransform(), // Current transform
        M_PI_2f,  // 45 degree
        {1.0f, 0.0f, 0.0f}  // y axies
    );

    bowser_arm_right->setGlobalTransform(rotatededArm);
    */


    // viewer.addRenderable(bowser);
    viewer.addRenderable(bowser_body);
    viewer.addRenderable(bowser_arm_right);
    viewer.addRenderable(bowser_arm_left);
    viewer.addRenderable(bowser_leg_right);
    viewer.addRenderable(bowser_leg_left);
    viewer.addRenderable(bowser_tail);
    viewer.addRenderable(bowser_eyes);





    // Create Lakitu
    const std::string lakitu_path = "../../sfmlGraphicsPipeline/meshes/lakitu.obj";

    auto lakitu = std::make_shared<TexturedLightedMeshRenderable>(textureShader, lakitu_path, myMaterial, "../../sfmlGraphicsPipeline/textures/lakitu.png");

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


    // Create Bowser's kart
    const std::string car_path = "../../sfmlGraphicsPipeline/meshes/kart/car.obj";
    const std::string canon_path = "../../sfmlGraphicsPipeline/meshes/kart/canon.obj";
    const std::string chain_path = "../../sfmlGraphicsPipeline/meshes/kart/chain.obj";
    const std::string eyes_path = "../../sfmlGraphicsPipeline/meshes/kart/eyes.obj";
    const std::string volant_centre_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_centre.obj";
    const std::string volant_tour_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_tour.obj";
    const std::string volant_barre_path = "../../sfmlGraphicsPipeline/meshes/kart/volant_barre.obj";
    const std::string gouvernaille_path = "../../sfmlGraphicsPipeline/meshes/kart/gouvernaille.obj";
    const std::string cylinder_back_path = "../../sfmlGraphicsPipeline/meshes/kart/cylinder_back.obj";
    const std::string cylinder_front_path = "../../sfmlGraphicsPipeline/meshes/kart/cylinder_front.obj";
    const std::string head_back_path = "../../sfmlGraphicsPipeline/meshes/kart/head_back.obj";
    const std::string hair_head_back_path = "../../sfmlGraphicsPipeline/meshes/kart/hair_head_back.obj";
    const std::string head_front_path = "../../sfmlGraphicsPipeline/meshes/kart/head_front.obj";
    const std::string tire_back_right_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_back_right.obj";
    const std::string tire_back_left_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_back_left.obj";
    const std::string tire_front_right_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_front_right.obj";
    const std::string tire_front_left_path = "../../sfmlGraphicsPipeline/meshes/kart/tire_front_left.obj";

    auto car = std::make_shared<TexturedLightedMeshRenderable>(textureShader, car_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto canon = std::make_shared<TexturedLightedMeshRenderable>(textureShader, canon_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto chain = std::make_shared<TexturedLightedMeshRenderable>(textureShader, chain_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto eyes = std::make_shared<TexturedLightedMeshRenderable>(textureShader, eyes_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/eyes.png");
    auto volant_centre = std::make_shared<TexturedLightedMeshRenderable>(textureShader, volant_centre_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto volant_tour = std::make_shared<TexturedLightedMeshRenderable>(textureShader, volant_tour_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto volant_barre = std::make_shared<TexturedLightedMeshRenderable>(textureShader, volant_barre_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto gouvernaille = std::make_shared<TexturedLightedMeshRenderable>(textureShader, gouvernaille_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto cylinder_back = std::make_shared<TexturedLightedMeshRenderable>(textureShader, cylinder_back_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto cylinder_front = std::make_shared<TexturedLightedMeshRenderable>(textureShader, cylinder_front_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto head_back = std::make_shared<TexturedLightedMeshRenderable>(textureShader, head_back_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto hair_head_back = std::make_shared<TexturedLightedMeshRenderable>(textureShader, hair_head_back_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto head_front = std::make_shared<TexturedLightedMeshRenderable>(textureShader, head_front_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto tire_back_right = std::make_shared<TexturedLightedMeshRenderable>(textureShader, tire_back_right_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto tire_back_left = std::make_shared<TexturedLightedMeshRenderable>(textureShader, tire_back_left_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto tire_front_right = std::make_shared<TexturedLightedMeshRenderable>(textureShader, tire_front_right_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");
    auto tire_front_left = std::make_shared<TexturedLightedMeshRenderable>(textureShader, tire_front_left_path, myMaterial, "../../sfmlGraphicsPipeline/textures/kart/kart.png");

    glm::mat4 kart_mat = getTranslationMatrix(20.0f, 1.144f, -1.6f) * getScaleMatrix(0.04f) * getRotationMatrix(M_PI_2f, {0.0f, 1.0f, 0.0f});
    car->setGlobalTransform(kart_mat);
    canon->setGlobalTransform(kart_mat);
    chain->setGlobalTransform(kart_mat);
    eyes->setGlobalTransform(kart_mat);
    volant_centre->setGlobalTransform(kart_mat);
    volant_tour->setGlobalTransform(kart_mat);
    volant_barre->setGlobalTransform(kart_mat);
    gouvernaille->setGlobalTransform(kart_mat);
    cylinder_back->setGlobalTransform(kart_mat);
    cylinder_front->setGlobalTransform(kart_mat);
    head_back->setGlobalTransform(kart_mat);
    hair_head_back->setGlobalTransform(kart_mat);
    head_front->setGlobalTransform(kart_mat);
    tire_back_right->setGlobalTransform(kart_mat);
    tire_back_left->setGlobalTransform(kart_mat);
    tire_front_right->setGlobalTransform(kart_mat);
    tire_front_left->setGlobalTransform(kart_mat);

    viewer.addRenderable(car);
    viewer.addRenderable(canon);
    viewer.addRenderable(chain);
    viewer.addRenderable(eyes);
    viewer.addRenderable(volant_centre);
    viewer.addRenderable(volant_tour);
    viewer.addRenderable(volant_barre);
    viewer.addRenderable(gouvernaille);
    viewer.addRenderable(cylinder_back);
    viewer.addRenderable(cylinder_front);
    viewer.addRenderable(head_back);
    viewer.addRenderable(hair_head_back);
    viewer.addRenderable(head_front);
    viewer.addRenderable(tire_back_right);
    viewer.addRenderable(tire_back_left);
    viewer.addRenderable(tire_front_right);
    viewer.addRenderable(tire_front_left);



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

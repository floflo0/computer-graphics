#include <ShaderProgram.hpp>
#include <Viewer.hpp>

#include <ShaderProgram.hpp>

#include <QuadMeshRenderable.hpp>
#include <FrameRenderable.hpp>

#include <dynamics/DynamicSystemRenderable.hpp>
#include <dynamics/DampingForceField.hpp>
#include <dynamics/ConstantForceField.hpp>
#include <dynamics/SpringForceField.hpp>
#include <dynamics/EulerExplicitSolver.hpp>

#include <dynamics/ParticleRenderable.hpp>
#include <dynamics/ParticleListRenderable.hpp>
#include <dynamics/ConstantForceFieldRenderable.hpp>
#include <dynamics/SpringForceFieldRenderable.hpp>
#include <dynamics/SpringListRenderable.hpp>
#include <dynamics/BobOmbExplosion.hpp>
#include <texturing/TexturedLightedMeshRenderable.hpp>
#include <texturing/CubeMapRenderable.hpp>

#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

void movingBobomb(Viewer& viewer, TexturedLightedMeshRenderablePtr &bobOmb);

void initialize_scene(Viewer &viewer) {
    // Set up a shader and add a 3D frame.
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl"
    );
    viewer.addShaderProgram(flatShader);

    ShaderProgramPtr textureShader = std::make_shared<ShaderProgram>(   "../../sfmlGraphicsPipeline/shaders/textureVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/textureFragment.glsl");
    viewer.addShaderProgram(textureShader);

    MaterialPtr myMaterial = Material::Bronze();

    viewer.addShaderProgram(flatShader);
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    ShaderProgramPtr cubeMapShader = std::make_shared<ShaderProgram>(  "../../sfmlGraphicsPipeline/shaders/cubeMapVertex.glsl",
                                                                    "../../sfmlGraphicsPipeline/shaders/cubeMapFragment.glsl");
    std::string cubemap_dir = "../../sfmlGraphicsPipeline/textures/skybox2";
    auto cubemap = std::make_shared<CubeMapRenderable>(cubeMapShader, cubemap_dir);

    viewer.addRenderable(cubemap);

    //lumières directionnelles (globale)
    glm::vec3 d_direction = glm::normalize(glm::vec3(0.0,-1.0,-1.0));
    glm::vec3 d_ambient(5.0,5.0,5.0), d_diffuse(0.3,0.3,0.1), d_specular(0.3,0.3,0.1);
    //glm::vec3 d_ambient(0.0,0.0,0.0), d_diffuse(0.0,0.0,0.0), d_specular(0.0,0.0,0.0);
    glm::vec3 lightPosition(0.0,5.0,8.0);
    DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);
    directionalLight->setGlobalTransform(getTranslationMatrix(lightPosition) * directionalLight->getGlobalTransform());

    viewer.addDirectionalLight(directionalLight);

    // Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

    // Create a renderable associated to the dynamic system
    // This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
    // It is also responsible for some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    // Create a ground plane, so that particles can bounce on it
    // To adapat with the point the bob-omb will explode
    glm::vec3 p1(10.0f, 0.0f, 10.0f); 
    glm::vec3 p2(10.0f, 0.0f, -10.0f);
    glm::vec3 p3(-10.0f, 0.0f, -10.0f);
    PlanePtr groundPlane = std::make_shared<Plane>(p1, p2, p3);
    system->addPlaneObstacle(groundPlane);


    // Create Bob-Omb
    const std::string bobOmb_path = "../../sfmlGraphicsPipeline/meshes/mk_objects/bob-omb.obj";

    auto bobOmb = std::make_shared<TexturedLightedMeshRenderable>(textureShader, bobOmb_path, myMaterial, "../../sfmlGraphicsPipeline/textures/mk_objects/bob-omb.png");

    bobOmb->setGlobalTransform(getTranslationMatrix(0.0f, 0.0f, 0.0f) * getRotationMatrix(-M_PI, 0.0f, 1.0f, 0.0f) * getScaleMatrix(0.1f));

    viewer.addRenderable(bobOmb);

    // Finally activate animation
    viewer.startAnimation();

    movingBobomb(viewer, bobOmb);

    //explosion(viewer, system, systemRenderable);

    auto bobOmbExplosion = std::make_shared<BobOmbExplosion>(
        system,
        1.2f,                  // trigger time
        bobOmb,
        systemRenderable,
        &viewer                // pass pointer to viewer
    );

    viewer.addRenderable(bobOmbExplosion);

}

int main() {
    glm::vec4 background_color(0.1f, 0.1f, 0.1f, 1.0f);
    Viewer viewer(1280, 720, background_color);
    initialize_scene(viewer);

    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

void movingBobomb(Viewer& viewer, TexturedLightedMeshRenderablePtr& bobOmbRenderable)
{
    const float scale = 0.1f;
    constexpr float epsilonScale = 0.0001f;

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 0.6f, -1.2f},
            qY(0.0f),
            glm::vec3(scale)
        ),
        0.0f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 1.8f, 0.3f},
            qY(0.4f),
            glm::vec3(scale)
        ),
        0.2f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 3.4f, 2.2f},
            qY(0.9f),
            glm::vec3(scale)
        ),
        0.4f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 4.2f, 4.0f},
            qY(1.4f),
            glm::vec3(scale)
        ),
        0.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 3.8f, 5.8f},
            qY(1.9f),
            glm::vec3(scale)
        ),
        0.8f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 2.6f, 7.8f},
            qY(2.6f),
            glm::vec3(scale)
        ),
        1.0f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 1.2f, 9.8f},
            qY(3.4f),
            glm::vec3(scale)
        ),
        1.2f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 0.4f, 11.4f},
            qY(4.2f),
            glm::vec3(scale)
        ),
        1.4f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 0.15f, 12.6f},
            qY(5.0f),
            glm::vec3(scale)
        ),
        1.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, 0.15f, 12.6f},
            qY(5.0f),
            glm::vec3(scale)
        ),
        1.6f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, -1000.0f, 0.0f},   // Push the bob-omb far below scene
            qY(5.0f),
            glm::vec3(0.0001f)
        ),
        1.7f
    );

    bobOmbRenderable->addGlobalTransformKeyframe(
        GeometricTransformation(
            {0.0f, -1000.0f, 0.0f},
            qY(5.0f),
            glm::vec3(0.0001f)
        ),
        200.0f // Freeze the animation, our film won't exceed 200 seconds
    );
}

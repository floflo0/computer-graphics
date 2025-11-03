#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <Viewer.hpp>
#include <QuadMeshRenderable.hpp>
#include <Plane.hpp>

void initialize_scene(Viewer &viewer) {
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
            "../../sfmlGraphicsPipeline/shaders/flatVertexNormal.glsl",
            "../../sfmlGraphicsPipeline/shaders/flatFragmentNormal.glsl");

    ShaderProgramPtr rainbowShader = std::make_shared<ShaderProgram>(
            "../../sfmlGraphicsPipeline/shaders/flatVertexNormal.glsl",
            "../../sfmlGraphicsPipeline/shaders/rainbowFragment.glsl");

    // Add the shader program to the viewer
    viewer.addShaderProgram(flatShader);
    viewer.addShaderProgram(rainbowShader);

    // Display axes

    std::string vShader =
        "./../../sfmlGraphicsPipeline/shaders/defaultVertex.glsl";
    std::string fShader =
        "./../../sfmlGraphicsPipeline/shaders/defaultFragment.glsl";
    ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader,
                                                                     fShader);
    // Add the shader program to the Viewer
    viewer.addShaderProgram(defaultShader);

    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(defaultShader);
    viewer.addRenderable(frame);


    // Create bowser
    const std::string bowser_path = "../../sfmlGraphicsPipeline/meshes/bowser_fixed.obj";
    MeshRenderablePtr bowser = std::make_shared<MeshRenderable>(flatShader, bowser_path);
    bowser->setModelMatrix(getTranslationMatrix(0.0f, -1.0f, -2.0f));

    viewer.addRenderable(bowser);

    // Rainbow road ?

    const glm::vec3 & p1 = glm::vec3(-1, -1, -5);
    const glm::vec3 & p2 = glm::vec3(1, -1, -5);
    const glm::vec3 & p3 = glm::vec3(1, -1, 5);
    const glm::vec3 & p4 = glm::vec3(-1, -1, 5);
    const glm::vec4 & color = glm::vec4(0.1, 0.8, 0.8, 1);
    
    QuadMeshRenderablePtr quad = std::make_shared<QuadMeshRenderable>(
        rainbowShader, p4, p3, p2, p1, color);

    // Add the quad to the viewer
    viewer.addRenderable(quad);
    
    //PlanePtr plane = std::make_shared<Plane>(p1, p2, p3); 

    //viewer.addRenderable(plane);
}

int main() {
    glm::vec4 background_color(0.2f, 0.2f, 0.2f, 1.0f);
    Viewer viewer(1280, 720, background_color);
    initialize_scene(viewer);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

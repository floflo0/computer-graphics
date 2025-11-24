#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <texturing/TexturedMeshRenderable.hpp>
#include <Viewer.hpp>
#include <Utils.hpp>

void initialize_scene(Viewer &viewer) {
    // Create a shader program
    ShaderProgramPtr  flatShader = std::make_shared<ShaderProgram>(
            "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
            "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl");

    // Add the shader program to the viewer
    viewer.addShaderProgram(flatShader);

    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/simpleTextureVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/simpleTextureFragment.glsl"
    );
    viewer.addShaderProgram(texShader);

    // Create bowser
    const std::string bowser_path = "../../sfmlGraphicsPipeline/meshes/bowser_fixed.obj";
    MeshRenderablePtr bowser = std::make_shared<MeshRenderable>(flatShader, bowser_path);
    bowser->setGlobalTransform(getTranslationMatrix(0.0f, 0.0f, 0.0f));
    // Add bowser to the viewer
    viewer.addRenderable(bowser);

    const std::string lakitu_path = "../../sfmlGraphicsPipeline/meshes/lakitu.obj";
    MeshRenderablePtr lakitu = std::make_shared<MeshRenderable>(flatShader, lakitu_path);
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

    glEnable(GL_CULL_FACE);
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

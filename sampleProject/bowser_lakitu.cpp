#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <MeshRenderable.hpp>
#include <ShaderProgram.hpp>
#include <Viewer.hpp>

void initialize_scene(Viewer &viewer) {
    // Create a shader program
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
            "../../sfmlGraphicsPipeline/shaders/flatVertexNormal.glsl",
            "../../sfmlGraphicsPipeline/shaders/flatFragmentNormal.glsl");

    // Add the shader program to the viewer
    viewer.addShaderProgram(flatShader);

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
}

int main() {
    glm::vec4 background_color(0.8f, 0.8f, 0.8f, 1.0f);
    Viewer viewer(1280, 720, background_color);
    initialize_scene(viewer);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

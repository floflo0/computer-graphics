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

    // Create a cylinder
    bool indexed = false; // indexed version already implemented
    unsigned int slices = 20u; // number of slices
    bool vertex_normals = true; // use vertex normals ? else triangle normals
                                 // See CylinderMeshRenderable.cpp
    CylinderMeshRenderablePtr cylinder = std::make_shared<CylinderMeshRenderable>(flatShader, indexed, slices, vertex_normals);

    // Add the cylinder to the viewer
    viewer.addRenderable(cylinder);

    const std::string cat_path = "../../sfmlGraphicsPipeline/meshes/cat.obj";
    MeshRenderablePtr cat = std::make_shared<MeshRenderable>(flatShader, cat_path);
    cat->setModelMatrix(getTranslationMatrix(-5.0f, 0.0f, 0.0f) *
                        getRotationMatrix(M_PI, {0.0f, 1.0f, 0.0f}));
    // Add cat to the viewer
    viewer.addRenderable(cat);

    const std::string pillar_path = "../../sfmlGraphicsPipeline/meshes/pillar.obj";
    MeshRenderablePtr pillar = std::make_shared<MeshRenderable>(flatShader, pillar_path);
    pillar->setModelMatrix(getTranslationMatrix(-5.0f, -3.2f, 0.5f) *
                           getRotationMatrix(M_PI_2, {0.0f, 0.0f, 1.0f}));
    // Add pillar to the viewer
    viewer.addRenderable(pillar);

    // Create suzanne
    const std::string suzanne_path = "../../sfmlGraphicsPipeline/meshes/suzanne.obj";
    MeshRenderablePtr suzanne = std::make_shared<MeshRenderable>(flatShader, suzanne_path);
    suzanne->setModelMatrix(getTranslationMatrix(5.0f, 0.0f, 0.0f) *
                            getRotationMatrix(M_PI, {0.0f, 1.0f, 0.0f}));
    // Add suzanne to the viewer
    viewer.addRenderable(suzanne);
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

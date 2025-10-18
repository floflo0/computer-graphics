#include <glm/glm.hpp>

#include <CubeRenderable.hpp>
#include <FrameRenderable.hpp>
#include <ShaderProgram.hpp>
#include <Viewer.hpp>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

int main() {
    // Stage 1: Create the window and its OpenGL context
    glm::vec4 background_color = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    Viewer viewer(SCREEN_WIDTH, SCREEN_HEIGHT, background_color);

    // Stage 2: Load resources like shaders, meshes... and make them part of the
    // virtual scene
    // Path to the vertex shader glsl code
    std::string vShader =
        "./../../sfmlGraphicsPipeline/shaders/defaultVertex.glsl";
    // Path to the fragment shader glsl code
    std::string fShader =
        "./../../sfmlGraphicsPipeline/shaders/defaultFragment.glsl";
    // Compile and link the shaders into a program
    ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader,
                                                                     fShader);
    // Add the shader program to the Viewer
    viewer.addShaderProgram(defaultShader);

    // Shader program instantiation
    // ...
    // When instantiating a renderable ,
    // you must specify the shader program used to draw it .
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(defaultShader);
    viewer.addRenderable(frame);

    vShader = "./../../sfmlGraphicsPipeline/shaders/flatVertex.glsl";
    // Path to the fragment shader glsl code
    fShader = "./../../sfmlGraphicsPipeline/shaders/flatFragment.glsl";
    // Compile and link the shaders into a program
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader,
                                                                  fShader);
    viewer.addShaderProgram(flatShader);

    // Instantiate a CubeRenderable while specifying its shader program
    CubeRenderablePtr cube = std::make_shared<CubeRenderable>(flatShader);

    // Add the renderable to the Viewer
    viewer.addRenderable(cube);

    // Stage 3: Our program loop
    while (viewer.isRunning()) {
        viewer.handleEvent();  // user interactivity (keyboard/mouse)
        viewer.draw();         // rasterization (write in framebuffer)
        viewer.display();      // refresh window
    }

    return EXIT_SUCCESS;
}

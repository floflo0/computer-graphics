#include <Viewer.hpp>
#include <ShaderProgram.hpp>
#include <FrameRenderable.hpp>
#include <HierarchicalRenderable.hpp>
#include <CylinderMeshRenderable.hpp>
#include <MeshRenderable.hpp>
#include <SphereMeshRenderable.hpp>

#define CYLINDER_SCALE 0.1f
#define CYLINDER_LENGHT 2.0f
#define ANGLE M_PI_4
#define SCALE 0.75f
#define NUMBER_CHILD 5

CylinderMeshRenderablePtr generateTree(ShaderProgramPtr flatShader, const int depth) {
    CylinderMeshRenderablePtr root = std::make_shared<CylinderMeshRenderable>(
        flatShader,
        false,
        20,
        true
    );
    root->setLocalTransform(getScaleMatrix(
        CYLINDER_SCALE,
        CYLINDER_LENGHT,
        CYLINDER_SCALE
    ));
    SphereMeshRenderablePtr sphere1 = std::make_shared<SphereMeshRenderable>(
        flatShader,
        true
    );
    SphereMeshRenderablePtr sphere2 = std::make_shared<SphereMeshRenderable>(
        flatShader,
        true
    );
    sphere1->setGlobalTransform(
        getTranslationMatrix(0.0f, CYLINDER_LENGHT * 0.5f, 0.0f)
        * getScaleMatrix(CYLINDER_SCALE)
    );
    sphere2->setGlobalTransform(
        getTranslationMatrix(0.0f, -CYLINDER_LENGHT * 0.5f, 0.0f) *
        getScaleMatrix(CYLINDER_SCALE)
    );
    HierarchicalRenderable::addChild(root, sphere1);
    HierarchicalRenderable::addChild(root, sphere2);
    if (depth > 1) {
        const int newDepth = depth - 1;
        for (size_t i = 0; i < NUMBER_CHILD; ++i) {
            CylinderMeshRenderablePtr child = generateTree(flatShader, newDepth);
            child->setGlobalTransform(
                getRotationMatrix(2.0f * M_PI * i / NUMBER_CHILD, 0.0f, 1.0f, 0.0f) *
                getTranslationMatrix(
                    -cosf(ANGLE) * CYLINDER_LENGHT * 0.5f * SCALE,
                    CYLINDER_LENGHT * 0.5f +
                        sinf(ANGLE) * CYLINDER_LENGHT * 0.5f * SCALE,
                    0.0f
                ) *
                getRotationMatrix(ANGLE, 0.0f, 0.0f, 1.0f) *
                getScaleMatrix(SCALE)
            );
            HierarchicalRenderable::addChild(root, child);
        }
    }
    return root;
}

void initialize_scene(Viewer& viewer) {
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
            "../../sfmlGraphicsPipeline/shaders/flatVertexNormal.glsl",
            "../../sfmlGraphicsPipeline/shaders/flatFragmentNormal.glsl");

    viewer.addShaderProgram(flatShader);

    viewer.addRenderable(generateTree(flatShader, 5));
}

int main() {
    Viewer viewer(1280, 720);
    initialize_scene(viewer);

    while(viewer.isRunning()) {
        viewer.handleEvent();
        viewer.animate();
        viewer.draw();
        viewer.display();
    }

    return EXIT_SUCCESS;
}

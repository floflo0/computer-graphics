#include <CylinderMeshRenderable.hpp>
#include <FrameRenderable.hpp>
#include <GeometricTransformation.hpp>
#include <ShaderProgram.hpp>
#include <SphereMeshRenderable.hpp>
#include <Viewer.hpp>

#define BRANCH_SCALE 0.1f
#define BRANCH_LENGHT 2.0f
#define ANGLE M_PI_4
#define CHILD_SCALE 0.75f
#define NUMBER_CHILD 3

void movingTree(Viewer& viewer);
void movingCylinder(Viewer& viewer);

void initialize_scene(Viewer& viewer) {
    // movingCylinder(viewer);
    movingTree(viewer);
}

int main() {
    glm::vec4 background_color(0.0f, 0.0f, 0.0f, 1.0f);
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

glm::quat qY(const float radians) {
    return glm::angleAxis(radians, glm::vec3(0.0f, 1.0f,0.0f));
}

glm::quat qZ(const float radians) {
    return glm::angleAxis(radians, glm::vec3(0.0f, 0.0f, 1.0f));
}

void movingCylinder(Viewer &viewer) {
    // Add shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragment.glsl"
    );
    viewer.addShaderProgram(flatShader);

    //Frame
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Animated cylinder
    auto cylinder = std::make_shared<CylinderMeshRenderable>(flatShader, false);
    cylinder->setGlobalTransform(glm::mat4(1.0));

    cylinder->addGlobalTransformKeyframe(GeometricTransformation(
        {0.0f, 0.0f, 0.0f},
        qY(0.0f),
        glm::vec3(1.0f)
    ), 0.0f);
    cylinder->addGlobalTransformKeyframe(GeometricTransformation(
        {2.0f, 0.0f, 0.0f},
        qY(glm::half_pi<float>()),
        glm::vec3(1.0f)
    ), 0.1f);
    cylinder->addGlobalTransformKeyframe(
        GeometricTransformation(
        {2.0f, 0.0f, 2.0f},
        qY(glm::pi<float>()),
        glm::vec3(1.0f)
    ), 0.2f);
    cylinder->addGlobalTransformKeyframe(GeometricTransformation(
        {0.0f, 0.0f, 2.0f},
        qY(3.0f * glm::half_pi<float>()),
        glm::vec3(1.0f)
    ), 0.3f);
    cylinder->addGlobalTransformKeyframe(GeometricTransformation(
        {0.0f, 0.0f, 0.0f},
        qY(2.0f * glm::pi<float>()),
        glm::vec3(1.0f)
    ), 0.4f);

    cylinder->addLocalTransformKeyframe(GeometricTransformation(
        {0.0f,0.0f,0.0f},
        qZ(0.f),
        {0.8f, 0.8f, 1.2f}
    ), 0.0f);
    cylinder->addLocalTransformKeyframe(GeometricTransformation(
        {0.0f,0.0f,0.0f},
        qZ(glm::pi<float>() / 2.0f),
        {1.2f, 1.2f, 0.9f}
    ), 0.1f);
    cylinder->addLocalTransformKeyframe(GeometricTransformation(
        {0.0f,0.0f,0.0f},
        qZ(glm::pi<float>()),
        {0.8f, 0.8f, 1.1f}
    ), 0.2f);
    cylinder->addLocalTransformKeyframe(GeometricTransformation(
        {0.0f,0.0f,0.0f},
        qZ(3.0f * glm::pi<float>() / 2.0f),
        {1.2f, 1.2f, 0.9f}
    ), 0.3f);
    cylinder->addLocalTransformKeyframe(GeometricTransformation(
        {0.0f,0.0f,0.0f},
        qZ(2.0f * glm::pi<float>()),
        {0.8f, 0.8f, 1.2f}
    ), 0.4f);

    viewer.addRenderable(cylinder);

    viewer.startAnimation();
}

SphereMeshRenderablePtr generateTree(ShaderProgramPtr flatShader,
                                     const int depth) {
    SphereMeshRenderablePtr root = std::make_shared<SphereMeshRenderable>(
        flatShader,
        true,
        5,
        10
    );
    root->setLocalTransform(getScaleMatrix(BRANCH_SCALE));
    CylinderMeshRenderablePtr cylinder =
        std::make_shared<CylinderMeshRenderable>(flatShader, false, 10, true);
    cylinder->setLocalTransform(
        getTranslationMatrix(0.0f, 1.0f, 0.0f) *
        getScaleMatrix(BRANCH_SCALE, BRANCH_LENGHT, BRANCH_SCALE)
    );
    SphereMeshRenderablePtr sphere = std::make_shared<SphereMeshRenderable>(
        flatShader,
        true,
        5,
        10
    );
    sphere->setLocalTransform(
        getTranslationMatrix(0.0f, BRANCH_LENGHT, 0.0f) *
        getScaleMatrix(BRANCH_SCALE)
    );
    HierarchicalRenderable::addChild(root, cylinder);
    HierarchicalRenderable::addChild(root, sphere);
    if (depth > 1) {
        for (size_t i = 0; i < NUMBER_CHILD; ++i) {
            auto child = generateTree(flatShader, depth - 1);
            const float wind_angle = (depth % 2 ? 1.0f : -1.0f) *
                                     glm::quarter_pi<float>();
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(0.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 0.0f);
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(wind_angle / 4.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 1.0f);
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(wind_angle / 8.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 1.5f);
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(wind_angle / 4.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 2.0f);
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(0.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 3.0f);
            child->addGlobalTransformKeyframe(GeometricTransformation(
                glm::vec3(0.0f, BRANCH_LENGHT, 0.0f),
                qZ(0.0f) *
                qY(2.0f * M_PI * i / NUMBER_CHILD) * qZ(ANGLE),
                glm::vec3(CHILD_SCALE)
            ), 3.5f);
            HierarchicalRenderable::addChild(root, child);
        }
    }
    return root;
}

void movingTree(Viewer& viewer) {
    //Add shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/flatVertexNormal.glsl",
        "../../sfmlGraphicsPipeline/shaders/flatFragmentNormal.glsl"
    );
    viewer.addShaderProgram(flatShader);
    viewer.addRenderable(generateTree(flatShader, 5));
    viewer.startAnimation();
}

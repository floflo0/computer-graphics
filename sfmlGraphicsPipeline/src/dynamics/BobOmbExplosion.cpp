#include <../../include/dynamics/BobOmbExplosion.hpp>

#include <../../include/dynamics/ConstantForceField.hpp>
#include <../../include/dynamics/DampingForceField.hpp>
#include <../../include/dynamics/ParticleListRenderable.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>


void explosion(Viewer& viewer,
               DynamicSystemPtr& system,
               DynamicSystemRenderablePtr& systemRenderable);


void BobOmbExplosion::onTrigger()
{
    if (m_triggered)
        return;

    m_triggered = true;

    // Trigger explosion
    explosion(*m_viewer, m_system, m_systemRenderable);
}

void explosion(Viewer& viewer,
               DynamicSystemPtr& system,
               DynamicSystemRenderablePtr& systemRenderable)
{
    ShaderProgramPtr instancedShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/instancedVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/instancedFragment.glsl"
    );
    viewer.addShaderProgram(instancedShader);

    const int numParticles = 100;
    const float pm = 1.0f;

    // This value will depend on the explosion position of the bob-omb
    const glm::vec3 px(0.0f, 0.15f, 12.6f);

    std::vector<ParticlePtr> particles(numParticles);

    for (int i = 0; i < numParticles; ++i)
    {
        glm::vec3 dir = glm::sphericalRand(1.0f);
        float pr = glm::linearRand(0.02f, 0.05f);
        float speed = glm::linearRand(5.0f, 10.0f);

        glm::vec3 pv = dir * speed;

        particles[i] = std::make_shared<Particle>(px, pv, pm, pr);
        system->addParticle(particles[i]);
    }

    // Gravity only  for explosion particles
    ConstantForceFieldPtr gravity =
        std::make_shared<ConstantForceField>(particles, DynamicSystem::gravity);
    system->addForceField(gravity);

    // Air damping only for explosion particles
    DampingForceFieldPtr damping =
        std::make_shared<DampingForceField>(particles, 2.0f);
    system->addForceField(damping);

    // Rendering
    ParticleListRenderablePtr particleListRenderable =
        std::make_shared<ParticleListRenderable>(instancedShader, particles);

    HierarchicalRenderable::addChild(systemRenderable, particleListRenderable);
}

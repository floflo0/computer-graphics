#include <../../include/dynamics/BobOmbExplosion.hpp>

#include <../../include/dynamics/ConstantForceField.hpp>
#include <../../include/dynamics/DampingForceField.hpp>
#include <../../include/dynamics/ParticleListRenderable.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>


void BobOmbExplosion::onTrigger_1()
{
    if (m_triggered_1)
        return;

    m_triggered_1 = true;

    // Trigger explosion
    explosion();
}

void BobOmbExplosion::onTrigger_2()
{
    if (m_triggered_2)
        return;

    m_triggered_2 = true;

    for (auto& p : particles)
    {
        p->setPosition(glm::vec3(0.0f, -1000.0f, 0.0f)); // move far away
    }
}


void BobOmbExplosion::explosion()
{
    ShaderProgramPtr instancedShader = std::make_shared<ShaderProgram>(
        "../../sfmlGraphicsPipeline/shaders/instancedVertex.glsl",
        "../../sfmlGraphicsPipeline/shaders/instancedFragment.glsl"
    );
    m_viewer->addShaderProgram(instancedShader);

    const float pm = 1.0f;
    const glm::vec3 px(37.5f, 2.3f, -46.8f);

    particles.resize(numParticles);

    for (int i = 0; i < numParticles; ++i)
    {
        glm::vec3 dir = glm::sphericalRand(1.0f);
        float pr = glm::linearRand(0.02f, 0.05f);
        float speed = glm::linearRand(5.0f, 10.0f);

        glm::vec3 pv = dir * speed;

        particles[i] = std::make_shared<Particle>(px, pv, pm, pr);
        m_system->addParticle(particles[i]);
    }

    // Gravity
    ConstantForceFieldPtr gravity =
        std::make_shared<ConstantForceField>(particles, DynamicSystem::gravity);
    m_system->addForceField(gravity);

    // Air damping
    DampingForceFieldPtr damping =
        std::make_shared<DampingForceField>(particles, 2.0f);
    m_system->addForceField(damping);

    // Rendering
    ParticleListRenderablePtr particleListRenderable =
        std::make_shared<ParticleListRenderable>(instancedShader, particles);

    HierarchicalRenderable::addChild(m_systemRenderable, particleListRenderable);
}

#pragma once
#include "DelayTriggerRenderable.hpp"
#include "../texturing/TexturedLightedMeshRenderable.hpp"
#include "../Viewer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

class BobOmbExplosion : public DelayTriggerRenderable {
public:
    BobOmbExplosion(DynamicSystemPtr system,
                    float triggerTime_1,
                    float triggerTime_2,
                    TexturedLightedMeshRenderablePtr bobOmb,
                    DynamicSystemRenderablePtr systemRenderable,
                    Viewer* viewer)
        : DelayTriggerRenderable(system, triggerTime_1, triggerTime_2),
          m_bobOmb(bobOmb),
          m_system(system),
          m_systemRenderable(systemRenderable),
          m_viewer(viewer)
    {}

protected:
    void onTrigger_1() override;
    void onTrigger_2() override;
    bool m_triggered_1 = false;
    bool m_triggered_2 = false;
    const int numParticles = 100;
    std::vector<ParticlePtr> particles;

private:
    void explosion();

    TexturedLightedMeshRenderablePtr m_bobOmb;
    DynamicSystemPtr m_system;
    DynamicSystemRenderablePtr m_systemRenderable;
    Viewer* m_viewer;
};

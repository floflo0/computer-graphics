#pragma once
#include "DelayTriggerRenderable.hpp"
#include "../texturing/TexturedLightedMeshRenderable.hpp"
#include "../Viewer.hpp"

class BobOmbExplosion : public DelayTriggerRenderable {
public:
    BobOmbExplosion(DynamicSystemPtr system,
                    float triggerTime,
                    TexturedLightedMeshRenderablePtr bobOmb,
                    DynamicSystemRenderablePtr systemRenderable,
                    Viewer* viewer)
        : DelayTriggerRenderable(system, triggerTime),
          m_bobOmb(bobOmb),
          m_system(system),
          m_systemRenderable(systemRenderable),
          m_viewer(viewer)
    {}

protected:
    void onTrigger() override;
    bool m_triggered = false;

private:
    TexturedLightedMeshRenderablePtr m_bobOmb;
    DynamicSystemPtr m_system;
    DynamicSystemRenderablePtr m_systemRenderable;
    Viewer* m_viewer;
};

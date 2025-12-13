#pragma once
#include "DynamicSystemRenderable.hpp"

class DelayTriggerRenderable : public DynamicSystemRenderable {
public:
    DelayTriggerRenderable(DynamicSystemPtr system, float triggerTime)
        : DynamicSystemRenderable(system),
          m_triggerTime(triggerTime),
          m_triggered(false)
    {}

    void do_animate(float time) override {
        DynamicSystemRenderable::do_animate(time);

        if (!m_triggered && time >= m_triggerTime) {
            m_triggered = true;
            onTrigger(); // call the object specific method
        }
    }

    bool isTriggered() const { return m_triggered; }

protected:
    virtual void onTrigger() = 0; 
    float m_triggerTime;

private:
    bool m_triggered;
};

#pragma once
#include "DynamicSystemRenderable.hpp"

class DelayTriggerRenderable : public DynamicSystemRenderable {
public:
    DelayTriggerRenderable(DynamicSystemPtr system, float triggerTime_1, float triggerTime_2)
        : DynamicSystemRenderable(system),
          m_triggerTime_1(triggerTime_1),
          m_triggerTime_2(triggerTime_2),
          m_triggered_1(false),
          m_triggered_2(false)
    {}

    void do_animate(float time) override {
        DynamicSystemRenderable::do_animate(time);

        if (!m_triggered_1 && time >= m_triggerTime_1) {
            m_triggered_1 = true;
            onTrigger_1(); // call the object specific method
        }

        if (!m_triggered_2 && time >= m_triggerTime_2) {
            m_triggered_2 = true;
            onTrigger_2(); // call the object specific method
        }
    }

    bool isTriggered_1() const { return m_triggered_1; }

protected:
    virtual void onTrigger_1() = 0; 
    virtual void onTrigger_2() = 0; 
    float m_triggerTime_1;
    float m_triggerTime_2;

private:
    bool m_triggered_1;
    bool m_triggered_2;
};

#pragma once

#include <KeyframedHierarchicalRenderable.hpp>

/**
 * Empty Renderable used to simplify animations.
 */
class SkeletonRenderable : public KeyframedHierarchicalRenderable {
public:
    ~SkeletonRenderable();
    SkeletonRenderable(ShaderProgramPtr program);

protected:
    virtual void do_draw();
};

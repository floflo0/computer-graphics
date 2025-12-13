#pragma once

#include <HierarchicalRenderable.hpp>

/**
 * Empty Renderable used to simplify animations.
 */
class SkeletonRenderable : public HierarchicalRenderable {
public:
    ~SkeletonRenderable();
    SkeletonRenderable(ShaderProgramPtr program);

protected:
    virtual void do_draw();
};

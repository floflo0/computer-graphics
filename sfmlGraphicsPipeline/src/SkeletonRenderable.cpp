#include <SkeletonRenderable.hpp>

SkeletonRenderable::SkeletonRenderable(ShaderProgramPtr prog)
   : KeyframedHierarchicalRenderable(prog) {}

void SkeletonRenderable::do_draw() {}

SkeletonRenderable::~SkeletonRenderable() {}

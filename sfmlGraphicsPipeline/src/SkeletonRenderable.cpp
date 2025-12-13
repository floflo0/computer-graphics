#include <SkeletonRenderable.hpp>

SkeletonRenderable::SkeletonRenderable(ShaderProgramPtr prog)
   : HierarchicalRenderable(prog) {}

void SkeletonRenderable::do_draw() {}

SkeletonRenderable::~SkeletonRenderable() {}

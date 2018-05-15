#ifndef RENDERER_H
#define RENDERER_H

#include "typedef.h"
#include "renderenvironment.h"

namespace viscas
{
    class Renderer
    {
    public:
        virtual ~Renderer()
        {
            // nothing to do
        }

        virtual void render(const RenderEnvironment& environment) = 0;
        virtual void resize(int, int)
        {
            // nothing to do
        }

        virtual void setHalfEdgeMesh(shared_half_edge_mesh& halfEdgeMesh) = 0;
    };
} // namespace viscas
#endif // RENDERER_H

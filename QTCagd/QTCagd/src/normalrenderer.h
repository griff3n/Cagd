#ifndef NORMALRENDERER_H
#define NORMALRENDERER_H

#include "abstractrenderer.h"
#include "mesh.h"

namespace viscas
{
    class NormalRenderer : public AbstractRenderer
    {
    public:
        NormalRenderer();
        virtual ~NormalRenderer();

        void render(const RenderEnvironment& environment) override;
        void setHalfEdgeMesh(shared_half_edge_mesh& halfEdgeMesh) override;
    private:
        void createVertexNormalsMesh(const QVector3D &color);
        void createFaceNormalsMesh(const QVector3D &color);
        void findScaling();
        void renderVertexNormals(const RenderEnvironment& environment);
        void renderFaceNormals(const RenderEnvironment& environment);
    private:
        std::unique_ptr<QOpenGLShaderProgram> program;

        // Scaling value for all normals.
        float scaling;

        shared_mesh vertexNormalsMesh;
        shared_mesh faceNormalsMesh;
    };

    using shared_normal_renderer = std::shared_ptr<NormalRenderer>;
} // namespace viscas

#endif // NORMALRENDERER_H

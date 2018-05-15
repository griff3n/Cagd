#ifndef VERTEXRENDERER_H
#define VERTEXRENDERER_H

#include "abstractrenderer.h"
#include "framebuffer.h"
#include "mesh.h"

#include "normalrenderer.h"

namespace viscas
{
    class VertexRenderer : public AbstractRenderer
    {
        Q_OBJECT
    public:
        VertexRenderer();

        void render(const RenderEnvironment& environment) override;
        void resize(int, int) override;
        void setHalfEdgeMesh(shared_half_edge_mesh& halfEdgeMesh) override;

        void setNormalRenderer(shared_normal_renderer& normalRenderer);
    private:
        void createShadedMesh();
        void createIdMesh();
        void createEdgeMesh(shared_mesh &mesh, const std::vector<half_edge_mesh::edge_index_type> &edges, const QVector3D &color);
        void createVerticesMesh(const QVector3D &color);
        void createFramebuffer(int width, int height);

        void renderFirstPass(const RenderEnvironment &environment);
        void renderEdges(const RenderEnvironment &environment);
        void renderSecondPass(const RenderEnvironment &environment);
    private:
        shared_normal_renderer normalRenderer;

        std::unique_ptr<QOpenGLShaderProgram> firstPassProgram;
        std::unique_ptr<QOpenGLShaderProgram> secondPassProgram;
        std::unique_ptr<QOpenGLShaderProgram> phongProgram;
        std::unique_ptr<QOpenGLShaderProgram> vertexProgram;

        unique_framebuffer framebuffer;

        unique_mesh idMesh;
        shared_mesh shadedMesh;
        shared_mesh edgeMesh;
        shared_mesh selectedVerticesMesh;
        shared_mesh verticesMesh;
    };

    using shared_vertex_renderer = std::shared_ptr<VertexRenderer>;
    using unique_vertex_renderer = std::unique_ptr<VertexRenderer>;
} // namespace viscas

#endif // VERTEXRENDERER_H

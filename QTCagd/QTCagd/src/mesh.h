#ifndef MESH_H
#define MESH_H

#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

namespace viscas
{
    class Mesh
    {
    public:
        Mesh(GLenum type, std::unique_ptr<QOpenGLVertexArrayObject> vao, const std::vector<QOpenGLBuffer>& attributeBuffers, GLsizei indexCount);
        virtual ~Mesh();
        void render();
    protected:
        QOpenGLFunctions *functions;
        GLenum type;
        std::unique_ptr<QOpenGLVertexArrayObject> vao;
        std::vector<QOpenGLBuffer> attributeBuffers;
        GLsizei indexCount;
    };

    typedef std::shared_ptr<Mesh> shared_mesh;
    typedef std::unique_ptr<Mesh> unique_mesh;
} // namespace viscas
#endif // MESH_H

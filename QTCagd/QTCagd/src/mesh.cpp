#include "mesh.h"

using namespace viscas;

Mesh::Mesh(GLenum type, std::unique_ptr<QOpenGLVertexArrayObject> vao, const std::vector<QOpenGLBuffer>& attributeBuffers, GLsizei indexCount)
    : functions(QOpenGLContext::currentContext()->functions())
    , type(type)
    , vao(std::move(vao))
    , attributeBuffers(attributeBuffers)
    , indexCount(indexCount)
{
    // nothing to see
}

Mesh::~Mesh()
{
    vao->destroy();

    for (auto&& buffer : attributeBuffers)
        buffer.destroy();

    attributeBuffers.clear();
}

void Mesh::render()
{
    vao->bind();

    // #todo change UNSIGNED_INT to something more useful (e.g. an attribute)?
    functions->glDrawElements(type, indexCount, GL_UNSIGNED_INT, 0);

    vao->release();
}


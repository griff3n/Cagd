#ifndef TOOLS_H
#define TOOLS_H

#include <unordered_map>
#include <vector>
#include <memory>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include "typedef.h"
#include "mesh.h"
#include "topologytoolbox.h"

namespace viscas
{
    namespace tools
    {
        /* Calculate normal for the given vertex. The normal is the average of all incident face normals. */
        inline QVector3D calculateNormal(const shared_half_edge_mesh& mesh, half_edge_mesh::vertex_index_type index)
        {
            topology_toolbox toolbox(mesh);

            auto a      = toolbox.point(index);

            auto begin  = toolbox.halfEdge(index);
            auto he     = begin;

            QVector3D sum;

            do
            {
                he = toolbox.pair(he);
                auto b = toolbox.point(toolbox.vertex(he));
                he = toolbox.next(he);
                auto c = toolbox.point(toolbox.vertex(toolbox.next(he)));

                auto face = toolbox.face(he);

                if (!toolbox.hole(face))
                    sum += QVector3D::normal(a, c, b);
            } while (he != begin);

            sum.normalize();

            return sum;
        }

        namespace builder
        {
            class BufferBuilder
            {
            public:
                BufferBuilder()
                    : bufferType(QOpenGLBuffer::Type::VertexBuffer)
                    , usage(QOpenGLBuffer::UsagePattern::StaticRead)
                {
                    // nothing to do
                }

                BufferBuilder& type(QOpenGLBuffer::Type bufferType)
                {
                    this->bufferType = bufferType;
                    return *this;
                }

                BufferBuilder& usagePattern(QOpenGLBuffer::UsagePattern usage)
                {
                    this->usage = usage;
                    return *this;
                }

                template <typename data_type>
                QOpenGLBuffer build(const data_type& data) const
                {
                    QOpenGLBuffer buffer(bufferType);

                    if (!buffer.create())
                        throw std::runtime_error("could not create vertex buffer");

                    buffer.setUsagePattern(usage);
                    buffer.bind();
                    buffer.allocate(data.data(), (int) (data.size() * sizeof(data_type::value_type)));

                    return buffer;
                }
            private:
                QOpenGLBuffer::Type bufferType;
                QOpenGLBuffer::UsagePattern usage;
            };

            class VertexArrayObjectBuilder
            {
            public:
                std::unique_ptr<QOpenGLVertexArrayObject> build() const
                {
                    auto vao = std::unique_ptr<QOpenGLVertexArrayObject>(new QOpenGLVertexArrayObject());

                    if (!vao->create())
                        throw std::runtime_error("Could not create vertex array object");

                    return vao;
                }
            };

            class MeshBuilder
            {
            public:
                MeshBuilder()
                    : meshType(GL_TRIANGLES)
                    , meshIndexCount(0)
                {
                    // nothing to do
                }

                MeshBuilder& addAttributeBuffer(const QOpenGLBuffer& buffer)
                {
                    attributeBuffers.push_back(buffer);
                    return *this;
                }

                MeshBuilder& type(GLenum type)
                {
                    meshType = type;
                    return *this;
                }

                /* At this point the ownership will be moved to the builder! */
                MeshBuilder& vao(std::unique_ptr<QOpenGLVertexArrayObject> vao)
                {
                    meshVao = std::move(vao);
                    return *this;
                }

                MeshBuilder& indexCount(unsigned int indexCount)
                {
                    meshIndexCount = indexCount;
                    return *this;
                }

                unique_mesh build()
                {
                    Q_ASSERT(meshVao != nullptr);
                    Q_ASSERT(meshIndexCount != 0);
                    Q_ASSERT(!attributeBuffers.empty());

                    return std::unique_ptr<Mesh>(new Mesh(meshType, std::move(meshVao), attributeBuffers, meshIndexCount));
                }
            private:
                GLenum meshType;
                std::unique_ptr<QOpenGLVertexArrayObject> meshVao;
                std::vector<QOpenGLBuffer> attributeBuffers;
                GLsizei meshIndexCount;
            };

            class ShaderProgramBuilder
            {
            public:
                ShaderProgramBuilder& addShader(QOpenGLShader::ShaderType type, const QString& filename)
                {
                    shaders[type] = filename;
                    return *this;
                }

                std::unique_ptr<QOpenGLShaderProgram> build(QObject *parent = nullptr)
                {
                    auto program = std::unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram(parent));

                    for (auto pair : shaders)
                    {
                        if (!program->addShaderFromSourceFile(pair.first, pair.second))
                        {
                            throw std::runtime_error(program->log().toStdString());
                        }
                    }

                    if (!program->link())
                        throw std::runtime_error(program->log().toStdString());

                    return program;
                }
            private:
                std::map<QOpenGLShader::ShaderType, QString> shaders;
            };
        } // namespace builder

        inline void buildShaderProgram(std::unique_ptr<QOpenGLShaderProgram>& program, const QString& vertex, const QString& fragment)
        {
            viscas::tools::builder::ShaderProgramBuilder builder;

            builder.addShader(QOpenGLShader::Vertex, vertex);
            builder.addShader(QOpenGLShader::Fragment, fragment);

            program = std::move(builder.build());
        }

        inline void buildShaderProgram(std::unique_ptr<QOpenGLShaderProgram>& program, const QString& vertex, const QString& geometry, const QString& fragment)
        {
            viscas::tools::builder::ShaderProgramBuilder builder;

            builder.addShader(QOpenGLShader::Vertex, vertex);
            builder.addShader(QOpenGLShader::Geometry, geometry);
            builder.addShader(QOpenGLShader::Fragment, fragment);

            program = std::move(builder.build());
        }
    } // namespace tools
} // namespace viscas

#endif // TOOLS_H

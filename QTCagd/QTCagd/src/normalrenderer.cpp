#include "normalrenderer.h"
#include "tools.h"

using namespace viscas;

NormalRenderer::NormalRenderer()
    : scaling(1.0f)
{
    tools::buildShaderProgram(program, "shaders/line.vert", "shaders/line.geom", "shaders/line.frag");

    program->bind();
    program->setUniformValue("edgeColor", QVector3D(1.0, 1.0, 0.0));
    program->setUniformValue("thickness", 0.8f);
}

NormalRenderer::~NormalRenderer()
{
    // nothing to see
}

void NormalRenderer::render(const RenderEnvironment& environment)
{
    if (halfEdgeMesh)
    {
        renderVertexNormals(environment);
        renderFaceNormals(environment);
    }
}

void NormalRenderer::setHalfEdgeMesh(shared_half_edge_mesh &halfEdgeMesh)
{
    AbstractRenderer::setHalfEdgeMesh(halfEdgeMesh);

    findScaling();

    createVertexNormalsMesh({0.8f, 0.01f, 0.8f});
    createFaceNormalsMesh({0.8f, 0.01f, 0.8f});
}

void NormalRenderer::createVertexNormalsMesh(const QVector3D &color)
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;
    using normal_vector = std::vector<half_edge_mesh::normal_index_type>;

    vec3_vector positions;
    vec3_vector colors;
    gl_uint_vector indices;

    for (auto vertex : toolbox.vertices())
    {
        normal_vector normals;

        auto p = toolbox.limitPointOrPoint(vertex);

        if (toolbox.hasNormal(vertex))
        {
            normals.push_back(toolbox.normal(vertex));
        }

        for (auto halfEdge: toolbox.halfEdgesOutgoing(vertex))
        {
            if (toolbox.hasNormal(halfEdge))
            {
                normals.push_back(toolbox.normal(halfEdge));
            }
        }

        for (auto normal : normals)
        {
            auto n = toolbox.normal(normal);
            auto p2 = p + n * scaling;

            indices.push_back((gl_uint_vector::value_type) positions.size());
            positions.push_back(p);
            colors.push_back(color);

            indices.push_back((gl_uint_vector::value_type) positions.size());
            positions.push_back(p2);
            colors.push_back(color);
        }
    }

    // No vertex normals given... return
    if (indices.empty())
    {
        vertexNormalsMesh.reset();
        return;
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    program->bind();

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    program->enableAttributeArray("position");
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    auto colorBuffer = viscas::tools::builder::BufferBuilder().build(colors);
    program->enableAttributeArray("color");
    program->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.type(GL_LINES);
    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(colorBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    vertexNormalsMesh = meshBuilder.build();
}

void NormalRenderer::createFaceNormalsMesh(const QVector3D& color)
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;

    vec3_vector positions;
    vec3_vector colors;
    gl_uint_vector indices;

    for (auto face : toolbox.faces())
    {
        if (!toolbox.hasNormal(face))
            continue;

        // TODO That looks really bad. We should think about refactoring (renaming).
        auto n = toolbox.normal(toolbox.normal(face));

        QVector3D p;
        int i = 0;

        for (auto vertex : toolbox.vertices(face))
        {
            i++;
            p += toolbox.point(vertex);
        }

        p /= (float) i;

        auto p2 = p + n * scaling;

        indices.push_back((gl_uint_vector::value_type) positions.size());
        positions.push_back(p);
        colors.push_back(color);

        indices.push_back((gl_uint_vector::value_type) positions.size());
        positions.push_back(p2);
        colors.push_back(color);
    }

    // No face normals given... return
    if (indices.empty())
    {
        faceNormalsMesh.reset();
        return;
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    program->bind();

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    program->enableAttributeArray("position");
    program->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    auto colorBuffer = viscas::tools::builder::BufferBuilder().build(colors);
    program->enableAttributeArray("color");
    program->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.type(GL_LINES);
    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(colorBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    faceNormalsMesh = meshBuilder.build();
}

void NormalRenderer::findScaling()
{
    topology_toolbox toolbox(halfEdgeMesh);

    // Find min and max. Will be used to calculate a scaling for normals.
    float f = std::numeric_limits<float>::max();

    float minx = f, miny = f, minz = f;
    float maxx = -f, maxy = -f, maxz = -f;

    for (auto vertex : toolbox.vertices())
    {
        auto p = toolbox.point(vertex);

        minx = p.x() < minx ? p.x() : minx;
        miny = p.y() < miny ? p.y() : miny;
        minz = p.z() < minz ? p.z() : minz;

        maxx = p.x() > maxx ? p.x() : maxx;
        maxy = p.y() > maxy ? p.y() : maxy;
        maxz = p.z() > maxz ? p.z() : maxz;
    }

    QVector3D min(minx, miny, minz);
    QVector3D max(maxx, maxy, maxz);

    // Use 10% of the greatest extent for normal scaling.
    scaling = 0.1f * (max - min).length();
}

void NormalRenderer::renderVertexNormals(const RenderEnvironment &environment)
{
    if (!propertiesModel->isShowVertexNormals())
        return;

    if (!vertexNormalsMesh)
        return;

    glDepthFunc(GL_LEQUAL);
    applyTo(program, environment);
    vertexNormalsMesh->render();
}

void NormalRenderer::renderFaceNormals(const RenderEnvironment &environment)
{
    if (!propertiesModel->isShowFaceNormals())
        return;

    if (!faceNormalsMesh)
        return;

    glDepthFunc(GL_LEQUAL);
    applyTo(program, environment);
    faceNormalsMesh->render();
}

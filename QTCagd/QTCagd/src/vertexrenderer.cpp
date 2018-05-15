#include <QSurface>

#include "typedef.h"
#include "tools.h"
#include "vertexrenderer.h"


using namespace viscas;

VertexRenderer::VertexRenderer()
{
    tools::buildShaderProgram(firstPassProgram, "shaders/firstPass.vert", "shaders/firstPass.frag");
    tools::buildShaderProgram(secondPassProgram, "shaders/secondPass.vert", "shaders/secondPass.geom", "shaders/secondPass.frag");
    tools::buildShaderProgram(phongProgram, "shaders/phong.vert", "shaders/phong.frag");
    tools::buildShaderProgram(vertexProgram, "shaders/center.vert", "shaders/center.frag");

    secondPassProgram->bind();
    secondPassProgram->setUniformValue("thickness", 0.4f);

    phongProgram->bind();
    phongProgram->setUniformValue("lightDirection", QVector3D(0.0f, 1.0f, 2.0f).normalized());
    phongProgram->setUniformValue("shininess", 128.0f);
    phongProgram->setUniformValue("ambient", QVector3D(0.2f, 0.2f, 0.2f));
    phongProgram->setUniformValue("specular", QVector3D(0.2f, 0.2f, 0.2f));

    // Create framebuffer.
    auto size = QOpenGLContext::currentContext()->surface()->size();
    createFramebuffer(size.width(), size.height());

    glEnable(GL_PROGRAM_POINT_SIZE);
    glPointSize(4.0f);
}

void VertexRenderer::render(const viscas::RenderEnvironment &environment)
{
    if (halfEdgeMesh)
    {
        renderFirstPass(environment);
        renderSecondPass(environment);
    }
}

void VertexRenderer::resize(int width, int height)
{
    createFramebuffer(width, height);
}

void VertexRenderer::setHalfEdgeMesh(shared_half_edge_mesh &halfEdgeMesh)
{
    AbstractRenderer::setHalfEdgeMesh(halfEdgeMesh);

    createIdMesh();
    createShadedMesh();
    topology_toolbox toolbox(halfEdgeMesh);
    createEdgeMesh(edgeMesh, toolbox.edges(), {0.5f, 0.5f, 0.5f});
    createVerticesMesh({0.1f, 0.1f, 0.1f});
}

void VertexRenderer::setNormalRenderer(shared_normal_renderer &normalRenderer)
{
    this->normalRenderer = normalRenderer;
}

void VertexRenderer::createShadedMesh()
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;

    vec3_vector positions;
    vec3_vector normals;
    vec3_vector colors;

    gl_uint_vector indices;

    const half_edge_mesh::point_type color{0.9f, 0.9f, 0.9f};

    for (auto face : toolbox.faces())
    {
        if (toolbox.hole(face))
            continue;

        auto vertices   = toolbox.vertices(face);

        auto vertexA    = vertices[0];
        auto pointA     = toolbox.limitPointOrPoint(vertexA);

        auto numTriangles = (half_edge_mesh::index_type) vertices.size() - 2;

        // Take the normal calculated from the first three vertices for the whole face (not correct but the visual result is much nicer).
        auto normal = QVector3D::normal(pointA, toolbox.limitPointOrPoint(vertices[1]), toolbox.limitPointOrPoint(vertices[2]));

        for (half_edge_mesh::index_type i = 0; i < numTriangles; i++)
        {
            auto vertexB = vertices[i + 1];
            auto vertexC = vertices[i + 2];

            auto pointB = toolbox.limitPointOrPoint(vertexB);
            auto pointC = toolbox.limitPointOrPoint(vertexC);

            indices.push_back((unsigned int) positions.size());
            positions.push_back(pointA);
            normals.push_back(normal);
            colors.push_back(color);

            indices.push_back((unsigned int) positions.size());
            positions.push_back(pointB);
            normals.push_back(normal);
            colors.push_back(color);

            indices.push_back((unsigned int) positions.size());
            positions.push_back(pointC);
            normals.push_back(normal);
            colors.push_back(color);
        }
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    phongProgram->bind();

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    phongProgram->enableAttributeArray("position");
    phongProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    // Normal buffer.
    auto normalBuffer = viscas::tools::builder::BufferBuilder().build(normals);
    phongProgram->enableAttributeArray("normal");
    phongProgram->setAttributeBuffer("normal", GL_FLOAT, 0, 3);

    // Color buffer.
    auto colorBuffer = viscas::tools::builder::BufferBuilder().build(colors);
    phongProgram->enableAttributeArray("color");
    phongProgram->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(normalBuffer);
    meshBuilder.addAttributeBuffer(colorBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    shadedMesh = meshBuilder.build();
}

void VertexRenderer::createIdMesh()
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;

    vec3_vector positions;
    gl_uint_vector ids;

    gl_uint_vector indices;

    for (auto face : toolbox.faces())
    {
        if (toolbox.hole(face))
            continue;

        auto vertices   = toolbox.vertices(face);
        auto pointA     = toolbox.limitPointOrPoint(vertices[0]);
        auto id         = face.data + 1;

        auto numTriangles = (half_edge_mesh::index_type) vertices.size() - 2;

        for (half_edge_mesh::index_type i = 0; i < numTriangles; i++)
        {
            auto pointB = toolbox.limitPointOrPoint(vertices[i + 1]);
            auto pointC = toolbox.limitPointOrPoint(vertices[i + 2]);

            indices.push_back((GLuint) positions.size());
            ids.push_back((GLuint) id);
            positions.push_back(pointA);

            indices.push_back((GLuint) positions.size());
            ids.push_back((GLuint) id);
            positions.push_back(pointB);

            indices.push_back((GLuint) positions.size());
            ids.push_back((GLuint) id);
            positions.push_back(pointC);
        }
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    firstPassProgram->bind();

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    firstPassProgram->enableAttributeArray("position");
    firstPassProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    // Id buffer.
    auto idBuffer = viscas::tools::builder::BufferBuilder().build(ids);
    GLuint pos = glGetAttribLocation(firstPassProgram->programId(), "id");
    glEnableVertexAttribArray(pos);
    glVertexAttribIPointer(pos, 1, GL_UNSIGNED_INT, 0, 0);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(idBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    idMesh = meshBuilder.build();

}

void VertexRenderer::createEdgeMesh(shared_mesh &mesh, const std::vector<half_edge_mesh::edge_index_type> &edges, const QVector3D &color)
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;

    vec3_vector positions;
    vec3_vector colors;
    gl_uint_vector ids0;
    gl_uint_vector ids1;

    gl_uint_vector indices;

    for (auto edge : edges)
    {
        auto start  = toolbox.start(edge);
        auto end    = toolbox.end(edge);

        auto id0 = toolbox.face(start).data + 1;
        auto id1 = toolbox.face(end).data + 1;

        auto pos0 = toolbox.limitPointOrPoint(start);
        auto pos1 = toolbox.limitPointOrPoint(end);

        indices.push_back((unsigned int) positions.size());
        positions.push_back(pos0);
        ids0.push_back((GLuint) id0);
        ids1.push_back((GLuint) id1);
        colors.push_back(color);

        indices.push_back((unsigned int) positions.size());
        positions.push_back(pos1);
        ids0.push_back((GLuint) id0);
        ids1.push_back((GLuint) id1);
        colors.push_back(color);
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    secondPassProgram->bind();

    GLuint pos;

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    secondPassProgram->enableAttributeArray("position");
    secondPassProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    // ID0 buffer.
    auto id0Buffer = viscas::tools::builder::BufferBuilder().build(ids0);
    pos = glGetAttribLocation(secondPassProgram->programId(), "id0");
    glEnableVertexAttribArray(pos);
    glVertexAttribIPointer(pos, 1, GL_UNSIGNED_INT, 0, 0);

    // ID1 buffer.
    auto id1Buffer = viscas::tools::builder::BufferBuilder().build(ids1);
    pos = glGetAttribLocation(secondPassProgram->programId(), "id1");
    glEnableVertexAttribArray(pos);
    glVertexAttribIPointer(pos, 1, GL_UNSIGNED_INT, 0, 0);

    // Color buffer.
    auto colorBuffer = viscas::tools::builder::BufferBuilder().usagePattern(QOpenGLBuffer::UsagePattern::DynamicRead).build(colors);
    secondPassProgram->enableAttributeArray("color");
    secondPassProgram->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.type(GL_LINES);
    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(id0Buffer);
    meshBuilder.addAttributeBuffer(id1Buffer);
    meshBuilder.addAttributeBuffer(colorBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    if (mesh)
        mesh.reset();

    mesh = meshBuilder.build();
}

void VertexRenderer::createVerticesMesh(const QVector3D &color)
{
    topology_toolbox toolbox(halfEdgeMesh);

    // For convenience.
    using vec3_vector = std::vector<half_edge_mesh::point_type>;
    using gl_uint_vector = std::vector<GLuint>;

    vec3_vector positions;
    vec3_vector colors;
    gl_uint_vector ids;
    gl_uint_vector indices;

    for (auto vertex : toolbox.vertices())
    {
        auto face = toolbox.face(toolbox.halfEdge(vertex));
        auto point = toolbox.limitPointOrPoint(vertex);

        indices.push_back((GLuint) positions.size());
        positions.push_back(point);
        colors.push_back(color);
        ids.push_back((GLuint) (face.data + 1));
    }

    auto vao = tools::builder::VertexArrayObjectBuilder().build();

    vao->bind();
    vertexProgram->bind();

    GLuint pos;

    // Position buffer.
    auto positionBuffer = viscas::tools::builder::BufferBuilder().build(positions);
    vertexProgram->enableAttributeArray("position");
    vertexProgram->setAttributeBuffer("position", GL_FLOAT, 0, 3);

    // Color buffer.
    auto colorBuffer = viscas::tools::builder::BufferBuilder().usagePattern(QOpenGLBuffer::UsagePattern::DynamicRead).build(colors);
    vertexProgram->enableAttributeArray("color");
    vertexProgram->setAttributeBuffer("color", GL_FLOAT, 0, 3);

    // ID0 buffer.
    auto idBuffer = viscas::tools::builder::BufferBuilder().build(ids);
    pos = glGetAttribLocation(vertexProgram->programId(), "id");
    glEnableVertexAttribArray(pos);
    glVertexAttribIPointer(pos, 1, GL_UNSIGNED_INT, 0, 0);

    // Index buffer.
    auto indexCount = (GLsizei) indices.size();
    auto indexBuffer = viscas::tools::builder::BufferBuilder().type(QOpenGLBuffer::Type::IndexBuffer).build(indices);

    // The final mesh.
    auto meshBuilder = viscas::tools::builder::MeshBuilder();

    meshBuilder.type(GL_POINTS);
    meshBuilder.vao(std::move(vao));
    meshBuilder.addAttributeBuffer(positionBuffer);
    meshBuilder.addAttributeBuffer(colorBuffer);
    meshBuilder.addAttributeBuffer(idBuffer);
    meshBuilder.addAttributeBuffer(indexBuffer);
    meshBuilder.indexCount(indexCount);

    verticesMesh = meshBuilder.build();
}

void VertexRenderer::createFramebuffer(int width, int height)
{
    if (framebuffer)
        framebuffer.reset();

    framebuffer = std::unique_ptr<Framebuffer>(new Framebuffer(1, width, height));
}

void VertexRenderer::renderFirstPass(const RenderEnvironment &environment)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    framebuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    applyTo(firstPassProgram, environment);
    glDepthFunc(GL_LEQUAL);
    idMesh->render();
}

void VertexRenderer::renderEdges(const RenderEnvironment &environment)
{
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, framebuffer->getTextureId(0));

    secondPassProgram->bind();
    secondPassProgram->setUniformValue("idTexture", 0);

    // Draw unselected edges.
    applyTo(secondPassProgram, environment);
    edgeMesh->render();

    // Draw centers
    applyTo(vertexProgram, environment);
    vertexProgram->setUniformValue("idTexture", 0);
    verticesMesh->render();

    glDisable(GL_BLEND);
}

void VertexRenderer::renderSecondPass(const RenderEnvironment &environment)
{
    auto background = environment.background;
    glClearColor(background.redF(), background.greenF(), background.blueF(), background.alphaF());

    Framebuffer::bindDefault();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw vertex and face normals.
    normalRenderer->render(environment);

    // Draw shaded mesh.
    applyTo(phongProgram, environment);
    glDepthFunc(GL_LEQUAL);
    shadedMesh->render();

    // Draw edges.
    renderEdges(environment);
}


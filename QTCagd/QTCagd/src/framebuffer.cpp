#include <QDebug>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include "framebuffer.h"

using namespace viscas;

Framebuffer::Framebuffer(int textures, int width, int height)
    : width(width)
    , height(height)
{
    initializeOpenGLFunctions();

    // Create Framebuffer.
    glGenFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    // Create attachments.
    textureIds.resize(textures);
    glGenTextures(textures, textureIds.data());

    std::vector<GLenum> attachments;

    for (int i = 0; i < textures; i++)
    {
        GLint textureId = textureIds[i];

        // Bind texture and create it.
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, width, height, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, NULL);

        GLenum result = glGetError();

        if (result != GL_NO_ERROR)
        {
            qDebug() << result;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Attach texture to framebuffer.
        GLenum att = (GLenum) (GL_COLOR_ATTACHMENT0 + i);
        glFramebufferTexture2D(GL_FRAMEBUFFER, att, GL_TEXTURE_2D, textureId, 0);

        attachments.push_back(att);
    }

    // One more texture for depth.
    glGenTextures(1, &depthTextureId);
    glBindTexture(GL_TEXTURE_2D, depthTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    GLenum result = glGetError();

    if (result != GL_NO_ERROR)
    {
        qDebug() << result;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach depth texture to framebuffer.
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTextureId, 0);

    // Finally tell opengl the attachments to be drawn into...
    glDrawBuffers((GLsizei) attachments.size(), attachments.data());

    // We are don. Back to the default framebuffer.
    Framebuffer::bindDefault();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &id);
    glDeleteTextures((unsigned int) textureIds.size(), textureIds.data());
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void Framebuffer::bindDefault()
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    context->functions()->glBindFramebuffer(GL_FRAMEBUFFER, context->defaultFramebufferObject());
}

GLuint Framebuffer::getTextureId(int texture) const
{
    return textureIds[texture];
}

GLuint Framebuffer::getDepthTextureId() const
{
    return depthTextureId;
}

GLuint Framebuffer::getId() const
{
    return id;
}

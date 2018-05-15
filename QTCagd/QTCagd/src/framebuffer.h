#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <memory>
#include <QOpenGLFunctions_3_3_Core>

namespace viscas
{
    class Framebuffer : QOpenGLFunctions_3_3_Core
    {
    public:
        Framebuffer(int textures, int width, int height);
        virtual ~Framebuffer();
        void bind();

        /* For convenience. */
        static void bindDefault();

        GLuint getTextureId(int texture) const;
        GLuint getDepthTextureId() const;
        GLuint getId() const;
    private:
        GLuint id;
        std::vector<GLuint> textureIds;
        GLuint depthTextureId;

        int width;
        int height;
    };

    typedef std::shared_ptr<Framebuffer> shared_framebuffer;
    typedef std::unique_ptr<Framebuffer> unique_framebuffer;
} // namespace viscas

#endif // FRAMEBUFFER_H

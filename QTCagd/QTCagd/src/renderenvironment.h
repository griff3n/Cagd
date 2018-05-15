#ifndef RENDERENVIRONMENT_H
#define RENDERENVIRONMENT_H

#include <QVector2D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QColor>
#include <QOpenGLShaderProgram>

namespace viscas
{
    struct RenderEnvironment
    {
        QMatrix4x4 mvp;
        QMatrix4x4 mv;
        QMatrix3x3 nm;

        QVector4D viewport;
        QVector2D nearFar;

        // Background color of the viewport.
        QColor background;
    };

    inline void applyTo(std::unique_ptr<QOpenGLShaderProgram>& program, const RenderEnvironment& environment)
    {
        Q_ASSERT (program != nullptr);

        program->bind();
        program->setUniformValue("mvp", environment.mvp);
        program->setUniformValue("mv", environment.mv);
        program->setUniformValue("nm", environment.nm);
        program->setUniformValue("viewport", environment.viewport);
        program->setUniformValue("depthrange", environment.nearFar);
    }
}

#endif // RENDERENVIRONMENT_H

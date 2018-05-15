#ifndef ABSTRACTRENDERER_H
#define ABSTRACTRENDERER_H

#include <memory>

#include <QObject>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "selectionmodel.h"
#include "propertiesmodel.h"

namespace viscas
{
    /*
        Provides setters for halfEdgeMesh, selectionModel and propertiesModel. Also adds listener
        to the models.
    */
    class AbstractRenderer : public QObject, public Renderer, public QOpenGLFunctions_3_3_Core
    {
        Q_OBJECT
    public:
        AbstractRenderer();
        virtual ~AbstractRenderer();

        void setHalfEdgeMesh(shared_half_edge_mesh& halfEdgeMesh) override;
        virtual void setSelectionModel(shared_selection_model selectionModel);
        virtual void setPropertiesModel(viscas::shared_properties_model propertiesModel);
    public slots:
        virtual void selectionChanged();
        virtual void propertyChanged();
    protected:
        shared_half_edge_mesh halfEdgeMesh;

        shared_selection_model selectionModel;
        shared_properties_model propertiesModel;
    };
} // namespace viscas

#endif // ABSTRACTRENDERER_H

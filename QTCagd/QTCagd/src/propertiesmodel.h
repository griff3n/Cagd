#ifndef PROPERTIESMODEL_H
#define PROPERTIESMODEL_H

#include <QObject>
#include <memory>
#include "typedef.h"

namespace viscas
{
    class PropertiesModel : public QObject, half_edge_mesh::half_edge_mesh_traits
    {
        Q_OBJECT
    public:
        bool isShowVertexNormals() const;
        bool isShowFaceNormals() const;
    signals:
        void propertyChanged();
    public slots:
        void setShowVertexNormals(bool show);
        void setShowFaceNormals(bool show);
    private:
        bool showVertexNormals = false;
        bool showFaceNormals = false;
    };

    using shared_properties_model = std::shared_ptr<PropertiesModel>;
}
#endif // PROPERTIESMODEL_H

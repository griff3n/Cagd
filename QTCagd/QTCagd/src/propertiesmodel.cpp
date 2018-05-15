#include "propertiesmodel.h"

using namespace viscas;

bool PropertiesModel::isShowVertexNormals() const
{
    return showVertexNormals;
}

bool PropertiesModel::isShowFaceNormals() const
{
    return showFaceNormals;
}

void PropertiesModel::setShowVertexNormals(bool showVertexNormals)
{
    if (this->showVertexNormals == showVertexNormals)
        return;

    this->showVertexNormals = showVertexNormals;

    emit propertyChanged();
}

void PropertiesModel::setShowFaceNormals(bool showFaceNormals)
{
    if (this->showFaceNormals == showFaceNormals)
        return;

    this->showFaceNormals = showFaceNormals;

    emit propertyChanged();
}

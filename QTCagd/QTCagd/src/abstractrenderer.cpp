#include "abstractrenderer.h"

using namespace viscas;

AbstractRenderer::AbstractRenderer()
{
    initializeOpenGLFunctions();
}

AbstractRenderer::~AbstractRenderer()
{
    // nothing to see
}

void AbstractRenderer::setHalfEdgeMesh(shared_half_edge_mesh& halfEdgeMesh)
{
    this->halfEdgeMesh = halfEdgeMesh;
}

void AbstractRenderer::setSelectionModel(shared_selection_model selectionModel)
{
    this->selectionModel = selectionModel;

    connect(this->selectionModel.get(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()));
}

void AbstractRenderer::setPropertiesModel(shared_properties_model propertiesModel)
{
    this->propertiesModel = propertiesModel;

    connect(this->propertiesModel.get(), SIGNAL(propertyChanged()), this, SLOT(propertyChanged()));
}

void AbstractRenderer::selectionChanged()
{
    // Override if necessary.
}

void AbstractRenderer::propertyChanged()
{
    // Override if necessary.
}

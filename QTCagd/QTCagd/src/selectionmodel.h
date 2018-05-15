#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QObject>
#include <memory>
#include <unordered_set>
#include "typedef.h"

namespace viscas
{
    /* This model will contain all selected vertices, edges and faces. */
    class SelectionModel : public QObject, half_edge_mesh::half_edge_mesh_traits
    {
        Q_OBJECT
    public:
        SelectionModel();

        std::unordered_set<vertex_index_type> getVertices() const;
        std::unordered_set<edge_index_type> getEdges() const;
        std::unordered_set<face_index_type> getFaces() const;

    signals:
        void selectionChanged();
    public slots:
        void addVertex(vertex_index_type vertex);
        void addEdge(edge_index_type edge);
        void addFace(face_index_type face);

        /* If the vertex is already selected, the selection will be removed. Otherwise the vertex will be selected. */
        void addVertexConditional(vertex_index_type vertex);
        /* If the edge is already selected, the selection will be removed. Otherwise the edge will be selected. */
        void addEdgeConditional(edge_index_type edge);
        /* If the face is already selected, the selection will be removed. Otherwise the face will be selected. */
        void addFaceConditional(face_index_type face);

        void setVertex(vertex_index_type vertex);
        void setEdge(edge_index_type edge);
        void setFace(face_index_type face);

        void addVertices(const std::vector<vertex_index_type>& vertex);
        void addEdges(const std::vector<edge_index_type>& edge);
        void addFaces(const std::vector<face_index_type>& face);

        void clearVertices();
        void clearEdges();
        void clearFaces();

        void clearSelection();
    private:
        std::unordered_set<vertex_index_type> vertices;
        std::unordered_set<edge_index_type> edges;
        std::unordered_set<face_index_type> faces;
    };

    using shared_selection_model = std::shared_ptr<SelectionModel>;
} // namespace viscas

#endif // SELECTIONMODEL_H

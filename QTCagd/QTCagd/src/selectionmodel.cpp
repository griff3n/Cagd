#include "selectionmodel.h"

using namespace viscas;

template <typename Element>
auto addElement(std::unordered_set<Element>& elements, const std::vector<Element>& newElements)
{
    bool changed = false;

    for (auto element : newElements)
    {
        if (elements.insert(element).second)
            changed = true;
    }

    return changed;
}

template <typename Element>
inline bool addElement(std::unordered_set<Element>& elements, Element element)
{
    auto result = elements.insert(element);

    return result.second;
}

template <typename Element>
inline void addElementConditional(std::unordered_set<Element>& elements, Element element)
{
    auto result = elements.find(element);

    if (result != elements.end())
        elements.erase(result);
    else
        elements.insert(element);
}

template <typename Element>
inline auto clearElements(std::unordered_set<Element>& elements)
{
    if (elements.empty())
        return false;

    elements.clear();

    return true;
}

template <typename Element>
inline auto setElement(std::unordered_set<Element>& elements, Element element)
{
    bool changed = false;

    if (elements.size() == 1)
    {
        auto currentSelection = *elements.begin();

        if (currentSelection != element)
        {
            elements.clear();
            elements.insert(element);

            changed = true;
        }
    }
    else
    {
        elements.clear();
        elements.insert(element);

        changed = true;
    }

    return changed;
}

SelectionModel::SelectionModel()
{

}

std::unordered_set<SelectionModel::vertex_index_type> SelectionModel::getVertices() const
{
    return vertices;
}

std::unordered_set<SelectionModel::edge_index_type> SelectionModel::getEdges() const
{
    return edges;
}

std::unordered_set<SelectionModel::face_index_type> SelectionModel::getFaces() const
{
    return faces;
}

void SelectionModel::addVertex(half_edge_mesh_traits::vertex_index_type vertex)
{
    if (addElement(vertices, vertex))
        emit selectionChanged();
}

void SelectionModel::addEdge(half_edge_mesh_traits::edge_index_type edge)
{
    if (addElement(edges, edge))
        emit selectionChanged();
}

void SelectionModel::addFace(half_edge_mesh_traits::face_index_type face)
{
    if (addElement(faces, face))
        emit selectionChanged();
}

void SelectionModel::addVertexConditional(half_edge_mesh_traits::vertex_index_type vertex)
{
    addElementConditional(vertices, vertex);

    emit selectionChanged();
}

void SelectionModel::addEdgeConditional(half_edge_mesh_traits::edge_index_type edge)
{
    addElementConditional(edges, edge);

    emit selectionChanged();
}

void SelectionModel::addFaceConditional(half_edge_mesh_traits::face_index_type face)
{
    addElementConditional(faces, face);

    emit selectionChanged();
}

void SelectionModel::setVertex(half_edge_mesh_traits::vertex_index_type vertex)
{
    if (setElement(vertices, vertex))
        emit selectionChanged();
}

void SelectionModel::setEdge(half_edge_mesh_traits::edge_index_type edge)
{
    if (setElement(edges, edge))
        emit selectionChanged();
}

void SelectionModel::setFace(half_edge_mesh_traits::face_index_type face)
{
    if (setElement(faces, face))
        emit selectionChanged();
}

void SelectionModel::addVertices(const std::vector<half_edge_mesh_traits::vertex_index_type> &vertices)
{
    if (addElement(this->vertices, vertices))
        emit selectionChanged();
}

void SelectionModel::addEdges(const std::vector<half_edge_mesh_traits::edge_index_type> &edges)
{
    if (addElement(this->edges, edges))
        emit selectionChanged();
}

void SelectionModel::addFaces(const std::vector<half_edge_mesh_traits::face_index_type> &faces)
{
    if (addElement(this->faces, faces))
        emit selectionChanged();
}

void SelectionModel::clearVertices()
{
    if (clearElements(vertices))
        emit selectionChanged();
}

void SelectionModel::clearEdges()
{
    if (clearElements(edges))
        emit selectionChanged();}

void SelectionModel::clearFaces()
{
    if (clearElements(faces))
        emit selectionChanged();
}

void SelectionModel::clearSelection()
{
    bool changed = false;

    changed &= clearElements(vertices);
    changed &= clearElements(edges);
    changed &= clearElements(faces);

    if (changed)
        emit selectionChanged();
}

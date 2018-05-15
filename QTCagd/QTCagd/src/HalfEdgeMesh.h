#pragma once

#pragma once

#include <vector>

#include "index.h"
#include "topology.h"
#include "features.h"

namespace viscas
{
	template <typename IndexType, typename PointType>
	struct HalfEdgeMesh;

	/*!
	* \brief Feature trait class.
	*
	* Defines useful types for later use, based on IndexType.
	*/
	template <typename IndexType>
	struct features_trait
	{
		// feature types

		using vertex_feature_type = features::VertexFeature<IndexType>;
		using half_edge_feature_type = features::HalfEdgeFeature<IndexType>;
		using edge_feature_type = features::EdgeFeature<IndexType>;
		using face_feature_type = features::FaceFeature<IndexType>;
	};

	/*!
	* \brief Type trait class for HalfEdgeMesh. Provides an uniform interface.
	*
	* Useful types are defined, based on IndexType and PointType.
	*/
	template <typename IndexType, typename PointType>
	struct half_edge_mesh_traits : public features_trait<IndexType>
	{
		// type aliases

		using half_edge_mesh_type = HalfEdgeMesh<IndexType, PointType>;

		using index_type = IndexType;
		using point_type = PointType;

		// normal type

		using normal_type = features::detail::Normal<IndexType>;

		// types for topology elements

		using vertex_type = topology::detail::Vertex<IndexType>;
		using half_edge_type = topology::detail::HalfEdge<IndexType>;
		using edge_type = topology::detail::Edge<IndexType>;
		using face_type = topology::detail::Face<IndexType>;

		// index types

		using normal_index_type = Index<normal_type>;
		using vertex_index_type = Index<vertex_type>;
		using half_edge_index_type = Index<half_edge_type>;
		using edge_index_type = Index<edge_type>;
		using face_index_type = Index<face_type>;
	};

	/*!
	* \brief Halfedge Data Structure.
	*
	* # General
	* This class represents a polygonal object. It only contains data. Functions associated with navigation, creation
	* of new vertices, halfedges, etc. are located in \ref viscas::topology::TopologyToolbox.
	* This class is template based. You can provide a IndexType (int, unsinged int, short, ...) and a PointType (QVector3D, QVector4D, glm::vec3, ...).
	* But keep in mind, different classes cannot work together. This means a class with IndexType int is not compatible to a class instantiated with unsinged short.
	*
	* Normally you do not work directly with objects of this type. You usually use toolboxes that provide methods for manipulation. One
	* default toolbox is the \ref viscas::topology::TopologyToolbox which provides everything for navigation and creation of different
	* topology related things.
	*
	* # Indexing
	* The data structure (aka topology) is made of a bunch of vectors. Each for a different type: \ref viscas::topology::detail::Vertex,
	* \ref viscas::topology::detail::HalfEdge, \ref viscas::topology::detail::Edge and \ref viscas::topology::detail::Face.
	* The connection between the topology elements are based on indices (see \ref viscas::Index). This leads to an efficient memory management.
	*
	* # Features
	* Features are not part of the topology elements themselfs. They kept separated.
	* If you want to access the features of the 4th vertex you have to access
	* the 4th element of the vector \ref HalfEdgeMesh::vertexFeatures. The TopologyToolbox and other toolboxes
	* will provide different methods for access.
	*
	* ## Subdivision scheme features
	* Different subdivision schemes will add new features to the topology elements. They usually provide a toolbox for manipulation. See for example
	* \ref viscas::commands::detail::CatmullClarkToolbox for manipulation and \ref viscas::features::detail::CatmullClarkFeature which will be used in
	* \ref viscas::features::VertexFeature, \ref viscas::features::EdgeFeature and \ref viscas::features::FaceFeature.
	*
	*/
	template <typename IndexType, typename PointType>
	struct HalfEdgeMesh : public half_edge_mesh_traits<IndexType, PointType>
	{
		// attributes

		/*!
		* \brief Level of manipulation.
		*
		* This attribute can be used to express how many times the halfedge mesh was manipulated.
		*/
		unsigned int level = 0;

		/*!
		* \brief List of points.
		*
		* For every vertex there must be a exactly one point (in general a 3D point). Points can be
		* accessed directly or by using (the preferred methods) from \ref viscas::topology::TopologyToolbox.
		*
		* If you want to access the point for the 3rd vertex you have to access the 3rd point in this vector.
		*/
		std::vector<PointType> points;

		/*!
		* \brief List of limit points.
		*
		* Some subdivision schemes will be able to create limit points during the calculation. They should
		* be saved using limitPoints and not points. Using points for limit points will lead to wrong calculations.
		*
		* The visualization will use limit points if available.
		*/
		std::vector<PointType> limitPoints;

		/*!
		* \brief List of normals.
		*/
		std::vector<normal_type> normals;

		/*!
		* \brief List of vertices.
		*
		* Part of the halfedge data structure topology.
		*/
		std::vector<vertex_type> vertices;

		/*!
		* \brief List of halfedges.
		*
		* Part of the halfedge data strcture topology.
		*/
		std::vector<half_edge_type> halfEdges;

		/*!
		* \brief List of edges.
		*
		* Part of the halfedge data strcture topology.
		*/
		std::vector<edge_type> edges;

		/*!
		* \brief List of faces.
		*
		* Part of the halfedge data strcture topology.
		*/
		std::vector<face_type> faces;

		/*!
		* \brief List of vertex features.
		*/
		std::vector<vertex_feature_type> vertexFeatures;

		/*!
		* \brief List of halfedge features.
		*/
		std::vector<half_edge_feature_type> halfEdgeFeatures;

		/*!
		* \brief List of edge features.
		*/
		std::vector<edge_feature_type> edgeFeatures;

		/*!
		* \brief List of face features.
		*/
		std::vector<face_feature_type> faceFeatures;

		/*!
		* \brief Does this halfedge mesh has a limit surface?
		*/
		bool hasLimitSurface = false;
	};
} // namespace viscas

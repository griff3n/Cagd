#pragma once

#include <memory>
#include "halfedgemesh.h"

namespace viscas
{
    /*!
     * \brief Iterators for HalfEdgeMesh.
     */
	namespace iterator
	{
		namespace detail
		{
			template <typename T>
			class HalfEdgeIterator;

			template <typename T>
			class VertexIterator;

			template <typename T>
			class PointIterator;

			template <typename T, typename Iterator>
			class FaceCycleIterator;

            /*!
             * \brief Simple iterator to navigate from halfedge to halfedge using next until we reached the first one again.
             *
             * This iterator will return the visited halfedge.
             */
            template <typename IndexType, typename PointType>
            class HalfEdgeIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				HalfEdgeIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
					: mesh(mesh)
					, halfEdge(halfEdge)
					, begin(begin)
				{
				}

				auto operator!=(const HalfEdgeIterator& other) const
				{
					return halfEdge != other.halfEdge || begin != other.begin;
				}

				const auto& operator++()
				{
					halfEdge = mesh->halfEdges[halfEdge].next;
					begin = false;

					return *this;
				}

				auto operator*() const
				{
					return halfEdge;
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				half_edge_index_type halfEdge;

				bool begin;
			};

            /*!
             * \brief Iterator to navigate from halfedge to halfedge (using next) until we reached the first one again.
             *
             * This iterator will return the vertex of every visited halfedge.
             */
            template <typename IndexType, typename PointType>
            class VertexIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				VertexIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
					: mesh(mesh)
					, halfEdge(halfEdge)
					, begin(begin)
				{
				}

				auto operator!=(const VertexIterator& other) const
				{
					return halfEdge != other.halfEdge || begin != other.begin;
				}

				const auto& operator++()
				{
					halfEdge = mesh->halfEdges[halfEdge].next;
					begin = false;

					return *this;
				}

				auto operator*() const
				{
					return mesh->halfEdges[halfEdge].vertex;
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				half_edge_index_type halfEdge;

				bool begin;
			};

            /*!
             * \brief Iterator to navigate from halfedge to halfedge (using next) until we reached the first one again.
             *
             * This iterator will return the point of every visited halfedge.
             */
            template <typename IndexType, typename PointType>
            class PointIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				PointIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
					: mesh(mesh)
					, halfEdge(halfEdge)
					, begin(begin)
				{
				}

				auto operator!=(const PointIterator& other) const
				{
					return halfEdge != other.halfEdge || begin != other.begin;
				}

				const auto& operator++()
				{
					halfEdge = mesh->halfEdges[halfEdge].next;
					begin = false;

					return *this;
				}

				auto operator*() const
				{
					auto vertex = mesh->halfEdges[halfEdge].vertex;

					return mesh->points[vertex];
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				half_edge_index_type halfEdge;

				bool begin;
            };

            /*!
             * \brief Cycles through all elements of the given face.
             *
             * Cycles through all elements (denoted by the given Iterator type) for a given face.
             */
            template <template <typename, typename> class HalfEdgeMesh, typename Iterator, typename IndexType, typename PointType>
            class FaceCycleIterator<HalfEdgeMesh<IndexType, PointType>, Iterator>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				FaceCycleIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const face_index_type& face)
					: mesh(mesh)
					, face(face)
				{
				}

				Iterator begin() const
				{
					auto halfEdge = mesh->faces[face].halfEdge;
					return {mesh, halfEdge, true};
				}

				Iterator end() const
				{
					auto halfEdge = mesh->faces[face].halfEdge;
					return {mesh, halfEdge, false};
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				face_index_type face;
			};

			template <typename T>
			class VertexCycleVertexIterator;

			template <typename T>
			class VertexCyclePointIterator;

            template <typename T>
            class VertexCycleFaceIterator;

            template <typename IndexType, typename PointType>
            class VertexCycleVertexIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				VertexCycleVertexIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
					: mesh(mesh)
					, halfEdge(halfEdge)
					, begin(begin)
				{
				}

				auto operator!=(const VertexCycleVertexIterator& other) const
				{
					return halfEdge != other.halfEdge || begin != other.begin;
				}

				const auto& operator++()
				{
                    // TODO Possible bug... should it be pair and then next? What is this iterator doing!?
					halfEdge = mesh->halfEdges[halfEdge].next;
					halfEdge = mesh->halfEdges[halfEdge].pair;

					begin = false;

					return *this;
				}

				auto operator*() const
				{
					return mesh->halfEdges[halfEdge].vertex;
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				half_edge_index_type halfEdge;

				bool begin;
			};

            template <typename IndexType, typename PointType>
            class VertexCyclePointIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				VertexCyclePointIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
					: mesh(mesh)
					, halfEdge(halfEdge)
					, begin(begin)
				{
				}

				auto operator!=(const VertexCyclePointIterator& other) const
				{
					return halfEdge != other.halfEdge || begin != other.begin;
				}

				const auto& operator++()
				{
					halfEdge = mesh->halfEdges[halfEdge].next;
					halfEdge = mesh->halfEdges[halfEdge].pair;

					begin = false;

					return *this;
				}

				auto operator*() const
				{
					auto temp = mesh->halfEdges[halfEdge].vertex;
					return mesh->points[temp];
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				half_edge_index_type halfEdge;

				bool begin;
			};

            template <typename IndexType, typename PointType>
            class VertexCycleFaceIterator<HalfEdgeMesh<IndexType, PointType>>
                : half_edge_mesh_traits<IndexType, PointType>
            {
            public:
                VertexCycleFaceIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const half_edge_index_type& halfEdge, bool begin)
                    : mesh(mesh)
                    , halfEdge(halfEdge)
                    , begin(begin)
                {
                }

                auto operator!=(const VertexCycleFaceIterator& other) const
                {
                    return halfEdge != other.halfEdge || begin != other.begin;
                }

                const auto& operator++()
                {
                    halfEdge = mesh->halfEdges[halfEdge].next;
                    halfEdge = mesh->halfEdges[halfEdge].pair;

                    begin = false;

                    return *this;
                }

                auto operator*() const
                {
                    return mesh->halfEdges[halfEdge].face;
                }
            private:
                std::shared_ptr<half_edge_mesh_type> mesh;
                half_edge_index_type halfEdge;

                bool begin;
            };

			template <typename T, typename Iterator>
			class VertexCycleIterator;

            /*!
             * \brief Collects every element in the neighborhood of the given vertex.
             *
             * The Iterator type will determine which type of elements we will collect (vertex, halfedge, etc.).
             */
            template <template <typename, typename> class HalfEdgeMesh, typename Iterator, typename IndexType, typename PointType>
            class VertexCycleIterator<HalfEdgeMesh<IndexType, PointType>, Iterator>
                : half_edge_mesh_traits<IndexType, PointType>
			{
			public:
				VertexCycleIterator(const std::shared_ptr<half_edge_mesh_type>& mesh, const vertex_index_type& vertex)
					: mesh(mesh)
					, vertex(vertex)
				{
				}

				Iterator begin() const
				{
					auto v = mesh->vertices[vertex];
					auto halfEdge = mesh->halfEdges[v.halfEdge].pair;

					return{ mesh, halfEdge, true };
				}

				Iterator end() const
				{
					auto v = mesh->vertices[vertex];
					auto halfEdge = mesh->halfEdges[v.halfEdge].pair;

					return{ mesh, halfEdge, false };
				}
			private:
				std::shared_ptr<half_edge_mesh_type> mesh;
				vertex_index_type vertex;
			};

		} // namespace detail

        // Types for convenience.

		template <typename HalfEdgeMesh>
		using VertexCycle = detail::FaceCycleIterator <HalfEdgeMesh, detail::VertexIterator<HalfEdgeMesh>>;

		template <typename HalfEdgeMesh>
		using HalfEdgeCycle = detail::FaceCycleIterator <HalfEdgeMesh, detail::HalfEdgeIterator<HalfEdgeMesh>>;

		template <typename HalfEdgeMesh>
		using PointCycle = detail::FaceCycleIterator <HalfEdgeMesh, detail::PointIterator<HalfEdgeMesh>>;

		template <typename HalfEdgeMesh>
		using VertexVertexNeighborhood = detail::VertexCycleIterator <HalfEdgeMesh, detail::VertexCycleVertexIterator<HalfEdgeMesh>>;

		template <typename HalfEdgeMesh>
		using VertexPointNeighborhood = detail::VertexCycleIterator <HalfEdgeMesh, detail::VertexCyclePointIterator<HalfEdgeMesh>>;

        template <typename HalfEdgeMesh>
        using VertexFaceNeighborhood = detail::VertexCycleIterator <HalfEdgeMesh, detail::VertexCycleFaceIterator<HalfEdgeMesh>>;
    } // namespace iterator
} // namespace viscas

#pragma once

#include "index.h"

namespace viscas
{
	/* Namespace for all important topology elements of the half edge data structure. */
	namespace topology
	{
		namespace detail
		{
			// forward declarations

			template <typename IndexType>
			struct Vertex;

			template <typename IndexType>
			struct HalfEdge;

			template <typename IndexType>
			struct Edge;

			template <typename IndexType>
			struct Face;

			// declarations

			template <typename IndexType>
			struct Vertex
			{
				using index_type = IndexType;

				Index<HalfEdge<IndexType>> halfEdge;
			};

			template <typename IndexType>
			struct HalfEdge
			{
				using index_type = IndexType;

				Index<Vertex<IndexType>> vertex;

				Index<HalfEdge<IndexType>> next;
				Index<HalfEdge<IndexType>> prev;
				Index<HalfEdge<IndexType>> pair;

				Index<Face<IndexType>> face;
                Index<Edge<IndexType>> edge;
			};

			template <typename IndexType>
			struct Edge
			{
				using index_type = IndexType;


                Edge(Index<HalfEdge<IndexType>> start, Index<HalfEdge<IndexType>> end)
                    : start(start)
                    , end(end)
                {
                    // nothing to see
                }

				Index<HalfEdge<IndexType>> start;
				Index<HalfEdge<IndexType>> end;
			};

			template <typename IndexType>
			struct Face
			{
				using index_type = IndexType;

				Index<HalfEdge<IndexType>> halfEdge;
			};
		}
	} // namespace topology
} // namespace viscas

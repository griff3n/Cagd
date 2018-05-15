#pragma once

#include <QVector3D>
#include "topology.h"

namespace viscas
{
    /*!
     * \brief Namespace for all halfedge data structure features.
     */
    namespace features
	{
		namespace detail
		{
			template <typename Element>
            struct Feature
			{
				// Nothing to see.
			};

            /*!
             * \brief Base class for all features.
             */
			template <template <typename> class Element, typename IndexType>
            struct Feature<Element<IndexType>>
			{
                /*!
                 * \brief This is the element of the topology for which the features is defined.
                 */
				using element_type = Element<IndexType>;

                /*!
                 * \brief Type for the IndexType. This is for consistency.
                 */
				using index_type = IndexType;
			};

            /*!
             * \brief Extra feature class for catmull clark topology features (to keep things separated from the
             * default features).
             */
            template <typename IndexType>
            struct CatmullClarkFeature
            {
                /*!
                 * \brief This index is used during the catmull clark subdivision to keep track of
                 * newly created topology elements.
                 *
                 * This is only interesting during the catmull clark subdivision algorithm. When the
                 * algorithm is done, this index is irrelevant.
                 */
                Index<topology::detail::Vertex<IndexType>> index;
            };

            /*!
             * \brief Defines the type for normals that are saved inside Vertex, HalfEdge, Edge and Face.
             */
            template <typename IndexType>
            struct Normal
            {
                using index_type = IndexType;

                /*!
                 * \brief The encapsulated 3d vector.
                 */
                QVector3D data;

                /*!
                 * \brief Conversion function for Normal<IndexType> to QVector.
                 * \return The encapsulated 3d vector.
                 */
                operator QVector3D() const noexcept
                {
                    return data;
                }
            };

            /*!
             * \brief Extra feature class for loop (to keep things separated from the
             * default features).
             */
            template <typename IndexType>
            struct LoopFeature
            {
                /*!
                 * \brief This index is used during the loop subdivision to keep track of
                 * newly created topology elements.
                 *
                 * This is only interesting during the loop subdivision algorithm. When the
                 * algorithm is done, this index is irrelevant.
                 */
                Index<topology::detail::Vertex<IndexType>> index;
            };

            /*!
             * \brief Extra feature class for loop ternary (to keep things separated from the
             * default features).
             */
            template <typename IndexType>
            struct LoopTernaryFeature
            {
                /*!
                 * \brief This index is used during the loop subdivision to keep track of
                 * newly created topology elements.
                 *
                 * This is only interesting during the loop subdivision algorithm. When the
                 * algorithm is done, this index is irrelevant.
                 */
                Index<topology::detail::Vertex<IndexType>> indexA;

                /*!
                 * \brief This index is used during the loop ternary subdivision to keep track of
                 * newly created topology elements.
                 *
                 * This is only interesting during the loop ternary subdivision algorithm. When the
                 * algorithm is done, this index is irrelevant.
                 */
                Index<topology::detail::Vertex<IndexType>> indexB;
            };
		} // namespace detail

        /*!
         * \brief Default features for vertex.
         *
         * New features should be added in extra classes if they are not globally used. See for
         * example CatmullClarkFeature or LoopFeature.
         */
		template <typename IndexType>
        struct VertexFeature : detail::Feature<topology::detail::Vertex<IndexType>>
		{
            // Default features.

            /*!
             * \brief This flag should be set, if the vertex has a limit point.
             */
            bool limit = false;

            /*!
             * \brief Index to a normal.
             *
             * If this index is invalid, the visualization will generate normals by itself.
             */
            Index<detail::Normal<IndexType>> normal;

            // Extensions that are not part of the default (and will not be handled by TopologyToolkit).

            detail::CatmullClarkFeature<IndexType> catmullClark;
            detail::LoopFeature<IndexType> loop;
        };

        /*!
         * \brief Default features for HalfEdge.
         */
		template <typename IndexType>
        struct HalfEdgeFeature : detail::Feature<topology::detail::HalfEdge<IndexType>>
		{
            // Default features.

            /*!
             * \brief Indicates if we have an sharp halfedge.
             *
             * If this halfedge is sharp, the pair halfedge must be sharp too. This is also the case
             * for the edge that these two halfedges are part of.
             *
             * A border halfedge is always sharp (and so is the edge).
             */
            bool sharp  = false;

            /*!
             * \brief Indicates if we have an halfedge that is part of the border.
             *
             * Border halfedges are always sharp too!
             */
            bool border = false;

            /*!
             * \brief Index to a normal.
             *
             * If this index is invalid, the visualization will generate normals by itself.
             */
            Index<detail::Normal<IndexType>> normal;

            // Extensions that are not part of the default (and will not be handled by TopologyToolkit).

            detail::LoopFeature<IndexType> loop;
            detail::LoopTernaryFeature<IndexType> loopTernary;
        };

        /*!
         * \brief Default features for Edge.
         */
		template <typename IndexType>
        struct EdgeFeature : detail::Feature<topology::detail::Edge<IndexType>>
		{
            // Default features.

            /*!
             * \brief Indicates if we have an sharp edge.
             *
             * If this edge is sharp, the halfedges which made up these edge are sharp too.
             *
             * A border edge is always sharp (and so are the halfedges).
             */
            bool sharp  = false;

            /*!
             * \brief Indicates if we have an edge that is part of the border.
             *
             * Border edges are always sharp too!
             */
            bool border = false;

            /*!
             * \brief Knot interval.
             */
            float knotInterval = 1.0f;

            // Extensions that are not part of the default (and will not be handled by TopologyToolkit).

            detail::CatmullClarkFeature<IndexType> catmullClark;
		};

        /*!
         * \brief Default features for Face.
         */
		template <typename IndexType>
        struct FaceFeature : detail::Feature<topology::detail::Face<IndexType>>
		{
            // Default features.

            /*!
             * \brief Hole face or not.
             *
             * Faces can be holes. They are not part of the visible polygonal object and are generated
             * for convenience. Algorithms are easier to implement if the halfedge mesh consists of
             * hole faces too (no nullptr handling is required).
             */
			bool hole = false;

            /*!
             * \brief Index to a normal.
             *
             * If this index is invalid, the visualization will generate normals by itself.
             */
            Index<detail::Normal<IndexType>> normal;

            // Extensions that are not part of the default (and will not be handled by TopologyToolkit).

            detail::CatmullClarkFeature<IndexType> catmullClark;
            detail::LoopFeature<IndexType> loop;
        };
	} // namespace properties
} // namespace viscas

#pragma once

#include <memory>
#include <iostream>
#include <cassert>
#include <unordered_map>

#include "topology.h"
#include "halfedgemesh.h"
#include "iterator.h"
#include "features.h"

namespace viscas
{
	namespace topology
	{
		template <typename T>
		class TopologyToolbox;

        template <template <typename, typename> class HalfEdgeMesh, typename IndexType, typename PointType>
        class TopologyToolbox<HalfEdgeMesh<IndexType, PointType>>
            : protected half_edge_mesh_traits<IndexType, PointType>
		{
		public:
			TopologyToolbox(std::shared_ptr<half_edge_mesh_type> mesh)
				: mesh(mesh)
			{
                // Check if the mesh is set (and not null).
                assert (mesh);
			}

			auto addVertex()
			{
                auto index = addElement(mesh->vertices, mesh->vertexFeatures);
				mesh->points.emplace_back();
                mesh->limitPoints.emplace_back();

				return index;
			}

			auto addVertex(PointType point)
			{
                auto index = addElement(mesh->vertices, mesh->vertexFeatures);
				mesh->points.push_back(point);
                mesh->limitPoints.emplace_back();

				return index;
			}

            auto addLimitVertex(PointType limitPoint)
            {
                auto index = addElement(mesh->vertices, mesh->vertexFeatures);
                mesh->points.emplace_back();
                mesh->limitPoints.push_back(limitPoint);

                // set limit feature explicitly to true
                limit(index, true);

                return index;
            }

			auto addHalfEdge()
			{
                return addElement(mesh->halfEdges, mesh->halfEdgeFeatures);
			}
			
			auto addEdge()
			{
                return addElement(mesh->edges, mesh->halfEdgeFeatures);
			}

            auto addEdge(half_edge_index_type start, half_edge_index_type end)
            {
                auto size  = mesh->edges.size();
                auto index = edge_index_type{ static_cast<IndexType>(size) };

                mesh->edges.emplace_back(start, end);
                mesh->edgeFeatures.emplace_back();

                return index;
            }
			
			auto addFace()
			{
                return addElement(mesh->faces, mesh->faceFeatures);
			}

            auto addNormal()
            {
                return addElement(mesh->normals);
            }

            auto addNormal(const QVector3D& normal)
            {
                auto size = mesh->normals.size();
                auto index = normal_index_type { static_cast<IndexType>(size) };

                mesh->normals.push_back({normal});

                return index;
            }

            auto normal(normal_index_type normal) const
            {
                assert(valid(normal));

                return mesh->normals[normal].data;
            }

            auto normal(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return mesh->halfEdgeFeatures[halfEdge].normal;
            }

            auto normal(face_index_type face) const
            {
                assert(valid(face));

                return mesh->faceFeatures[face].normal;
            }

            auto normal(vertex_index_type vertex, normal_index_type normal)
            {
                assert(valid(vertex));

                mesh->vertexFeatures[vertex].normal = normal;
            }

            auto normal(half_edge_index_type halfEdge, normal_index_type normal)
            {
                assert(valid(halfEdge));

                mesh->halfEdgeFeatures[halfEdge].normal = normal;
            }

            auto normal(face_index_type face, normal_index_type normal)
            {
                assert(valid(face));

                mesh->faceFeatures[face].normal = normal;
            }

            auto normal(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                return mesh->vertexFeatures[vertex].normal;
            }

            auto hasNormal(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                return valid(mesh->vertexFeatures[vertex].normal);
            }

            auto hasNormal(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return valid(mesh->halfEdgeFeatures[halfEdge].normal);
            }

            auto hasNormal(face_index_type face) const
            {
                assert(valid(face));

                return valid(mesh->faceFeatures[face].normal);
            }

			auto halfEdge(vertex_index_type vertex) const
			{
				assert(valid(vertex));

				return mesh->vertices[vertex].halfEdge;
			}

			void halfEdge(vertex_index_type vertex, half_edge_index_type halfEdge) const
			{
				assert(valid(vertex));
				assert(valid(halfEdge));

				mesh->vertices[vertex].halfEdge = halfEdge;
			}

			auto halfEdge(face_index_type face) const
			{
				assert(valid(face));

				return mesh->faces[face].halfEdge;
			}

			void halfEdge(face_index_type face, half_edge_index_type halfEdge) const
			{
				assert(valid(face));
				assert(valid(halfEdge));

				mesh->faces[face].halfEdge = halfEdge;
			}

			auto next(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

				return mesh->halfEdges[halfEdge].next;
			}

			void next(half_edge_index_type halfEdge, half_edge_index_type next)
			{
				assert(valid(halfEdge));
				assert(valid(next));

				mesh->halfEdges[halfEdge].next = next;
			}

			auto prev(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

				return mesh->halfEdges[halfEdge].prev;
			}

			void prev(half_edge_index_type halfEdge, half_edge_index_type prev)
			{
				assert(valid(halfEdge));
				assert(valid(prev));

				mesh->halfEdges[halfEdge].prev = prev;
			}

			auto vertex(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

				return mesh->halfEdges[halfEdge].vertex;
			}

			void vertex(half_edge_index_type halfEdge, vertex_index_type vertex)
			{
				assert(valid(halfEdge));
				assert(valid(vertex));

				mesh->halfEdges[halfEdge].vertex = vertex;
			}

			auto face(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

				return mesh->halfEdges[halfEdge].face;
			}

			void face(half_edge_index_type halfEdge, face_index_type face)
			{
				assert(valid(halfEdge));
				assert(valid(face));

				mesh->halfEdges[halfEdge].face = face;
			}

			auto pair(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

				return mesh->halfEdges[halfEdge].pair;
			}

			void pair(half_edge_index_type halfEdge, half_edge_index_type pair)
			{
				assert(valid(halfEdge));
				assert(valid(pair));

				mesh->halfEdges[halfEdge].pair = pair;
			}

            auto start(edge_index_type edge) const
            {
                assert(valid(edge));

                return mesh->edges[edge].start;
            }

            void start(edge_index_type edge, half_edge_index_type start)
            {
                assert(valid(edge));

                mesh->edges[edge].start = start;
            }

            auto end(edge_index_type edge) const
            {
                assert(valid(edge));

                return mesh->edges[edge].end;
            }

            void end(edge_index_type edge, half_edge_index_type end)
            {
                assert(valid(edge));

                mesh->edges[edge].end = end;
            }

            auto edge(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return mesh->halfEdges[halfEdge].edge;
            }

            void edge(half_edge_index_type halfEdge, edge_index_type edge)
            {
                assert(valid(halfEdge));
                assert(valid(edge));

                mesh->halfEdges[halfEdge].edge = edge;
            }

            // point

            point_type point(vertex_index_type vertex) const
			{
				assert(valid(vertex));
				
				return mesh->points[vertex];
			}

            point_type point(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return point(vertex(halfEdge));
            }

            point_type point(edge_index_type edge, bool start) const
            {
                assert(valid(edge));

                auto halfEdge = start ? TopologyToolbox::start(edge) : end(edge);

                return point(vertex(halfEdge));
            }

            void point(vertex_index_type vertex, const point_type& point) const
			{
				assert(valid(vertex));

				mesh->points[vertex] = point;
			}

			auto points(face_index_type face) const
			{
				assert(valid(face));

                std::vector<point_type> result;

				auto start = halfEdge(face);
				auto he = start;

				do
				{
					result.push_back(point(vertex(he)));
					he = next(he);
				} while (he != start);

				return result;
			}

            // limit point

            point_type limitPoint(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                return mesh->limitPoints[vertex];
            }

            point_type limitPoint(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return limitPoint(vertex(halfEdge));
            }

            point_type limitPoint(edge_index_type edge, bool start) const
            {
                assert(valid(edge));

                auto halfEdge = start ? TopologyToolbox::start(edge) : end(edge);

                return limitPoint(vertex(halfEdge));
            }

            void limitPoint(vertex_index_type vertex, const PointType& limitPoint) const
            {
                assert(valid(vertex));

                mesh->limitPoints[vertex] = limitPoint;
            }

            auto limitPoints(face_index_type face) const
            {
                assert(valid(face));

                std::vector<point_type> result;

                auto start = halfEdge(face);
                auto he = start;

                do
                {
                    result.push_back(limitPoint(vertex(he)));
                    he = next(he);
                } while (he != start);

                return result;
            }

            // conditional limit point stuff (for convenience)

            point_type limitPointOrPoint(vertex_index_type vertex) const
            {
                assert (valid(vertex));

                if (limit(vertex))
                    return TopologyToolbox::limitPoint(vertex);

                return point(vertex);
            }

            point_type limitPointOrPoint(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                if (limit(vertex(halfEdge)))
                    return limitPoint(vertex(halfEdge));

                return point(vertex(halfEdge));
            }

            point_type limitPointOrPoint(edge_index_type edge, bool start) const
            {
                assert(valid(edge));

                auto halfEdge = start ? TopologyToolbox::start(edge) : end(edge);

                if (limit(vertex(halfEdge)))
                    return limitPoint(vertex(halfEdge));

                return point(vertex(halfEdge));
            }

            //

			auto vertices(face_index_type face) const
			{
				assert(valid(face));

				std::vector<vertex_index_type> result;

				auto start = halfEdge(face);
				auto he = start;

				do
				{
					result.push_back(vertex(he));
					he = next(he);
				} while (he != start);

				return result;
			}

            auto vertices(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<vertex_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    he = pair(he);
                    result.push_back(TopologyToolbox::vertex(he));
                    he = next(he);
                } while (he != start);

                return result;
            }

            auto sharpHalfEdges(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<half_edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    if (sharp(he))
                        result.push_back(he);

                    he = pair(he);

                    if (sharp(he))
                        result.push_back(he);

                    he = next(he);
                } while (he != start);

                return result;
            }

            auto sharpEdges(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    auto currentEdge = edge(he);

                    if (sharp(currentEdge))
                        result.push_back(currentEdge);

                    he = pair(he);
                    he = next(he);
                } while (he != start);

                return result;
            }
			
            auto halfEdges(face_index_type face) const
			{
				assert(valid(face));

				std::vector<half_edge_index_type> result;

				auto start = halfEdge(face);
				auto he = start;

				do
				{
					result.push_back(he);
					he = next(he);
				} while (he != start);

				return result;
            }

            /*
             * This method will return all sharp half edges that are incident with the given vertex and start from there.
             */
            auto sharpHalfEdgesOutgoing(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<half_edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    if (sharp(he)) {
                        result.push_back(he);
                    }

                    he = pair(he);
                    he = next(he);
                } while (he != start);

                return result;
            }
            /*
             * This method will return all half edges that are incident with the given vertex and start from there.
             */
            std::vector<half_edge_index_type> halfEdgesOutgoing(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<half_edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    result.push_back(he);

                    he = pair(he);
                    he = next(he);
                } while (he != start);

                return result;
            }

            /*
             * This method will return all half edges that are incident with the given vertex and end here.
             */
            std::vector<half_edge_index_type> halfEdgesIncoming(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<half_edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    he = pair(he);
                    result.push_back(he);
                    he = next(he);
                } while (he != start);

                return result;
            }

            auto vertices() const
            {
                return indices(mesh->vertices);
            }

            auto halfEdges() const
            {
                return indices(mesh->halfEdges);
            }            

            auto edges() const
            {
                return indices(mesh->edges);
            }

            auto edges(face_index_type face) const
            {
                assert(valid(face));

                std::vector<edge_index_type> result;

                auto start = halfEdge(face);
                auto he = start;

                do
                {
                    result.push_back(edge(he));
                    he = next(he);
                } while (he != start);

                return result;
            }

            auto edges(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                std::vector<edge_index_type> result;

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    auto currentEdge = edge(he);

                    result.push_back(currentEdge);

                    he = pair(he);
                    he = next(he);
                } while (he != start);

                return result;
            }

            auto faces() const
            {
                return indices(mesh->faces);
            }

			auto hole(face_index_type face) const
			{
				assert(valid(face));

                return mesh->faceFeatures[face].hole;
			}
			
			void hole(face_index_type face, bool hole) const
			{
				assert(valid(face));

                mesh->faceFeatures[face].hole = hole;
			}

            auto border(half_edge_index_type halfEdge) const
			{
				assert(valid(halfEdge));

                return mesh->halfEdgeFeatures[halfEdge].border;
			}

            void border(half_edge_index_type halfEdge, bool border)
			{
				assert(valid(halfEdge));

                mesh->halfEdgeFeatures[halfEdge].border = border;
			}

            auto border(edge_index_type edge) const
            {
                assert(valid(edge));

                return mesh->edgeFeatures[edge].border;
            }

            void border(edge_index_type edge, bool border)
            {
                assert(valid(edge));

                mesh->edgeFeatures[edge].border = border;
            }

            auto sharp(half_edge_index_type halfEdge) const
            {
                assert(valid(halfEdge));

                return mesh->halfEdgeFeatures[halfEdge].sharp;
            }

            void sharp(half_edge_index_type halfEdge, bool sharp)
            {
                assert(valid(halfEdge));

                mesh->halfEdgeFeatures[halfEdge].sharp = sharp;
            }

            auto sharp(edge_index_type edge) const
            {
                assert(valid(edge));

                return mesh->edgeFeatures[edge].sharp;
            }

            void sharp(edge_index_type edge, bool sharp)
            {
                assert(valid(edge));

                mesh->edgeFeatures[edge].sharp = sharp;
            }

            auto knotInterval(edge_index_type edge) const
            {
                assert(valid(edge));

                return mesh->edgeFeatures[edge].knotInterval;
            }

            void knotInterval(edge_index_type edge, float knotInterval)
            {
                assert(valid(edge));

                mesh->edgeFeatures[edge].knotInterval = knotInterval;
            }

            unsigned int level() const
            {
                return mesh->level;
            }

            void level(unsigned int level)
            {
                mesh->level = level;
            }

            index_type valence(face_index_type face) const
            {
                assert(valid(face));

                index_type result {0};

                auto start = halfEdge(face);
                auto he = start;

                do
                {
                    result++;
                    he = next(he);
                } while (he != start);

                return result;
            }

            index_type valence(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                index_type result {0};

                auto start = halfEdge(vertex);
                auto he = start;

                do
                {
                    result++;
                    he = pair(he);
                    he = next(he);
                } while (he != start);

                return result;
            }

            void createEdges()
            {
                // half edges that are already part of an edge
                std::unordered_set<half_edge_index_type> consumed;

                for (auto halfEdge : halfEdges())
                {
                    if (consumed.find(halfEdge) != consumed.end())
                        continue;

                    auto pair = TopologyToolbox::pair(halfEdge);
                    auto edge = addEdge(halfEdge, pair);

                    TopologyToolbox::edge(halfEdge, edge);
                    TopologyToolbox::edge(pair, edge);

                    consumed.insert(halfEdge);
                    consumed.insert(pair);

                    // copy features
                    assert (border(halfEdge) == border(pair));
                    assert (sharp(halfEdge) == sharp(pair));

                    border(edge, border(halfEdge));
                    sharp(edge, sharp(halfEdge));
                }
            }

            void connect(std::vector<half_edge_index_type> &halfEdges)
            {
                // Set next and prev
                auto current = *halfEdges.begin();
                auto last = *halfEdges.rbegin();

                prev(current, last);
                next(last, current);

                for (std::vector<half_edge_index_type>::iterator iter = std::next(halfEdges.begin()); iter != halfEdges.end(); ++iter)
                {
                    auto index = *iter;

                    next(current, index);
                    prev(index, current);

                    current = index;
                }
            }

            auto createHoleHalfEdges()
            {
                std::vector<half_edge_index_type> holeHalfEdges;

                for (auto halfEdge : halfEdges())
                {
                    auto pair = TopologyToolbox::pair(halfEdge);

                    if (valid(pair))
                        continue;

                    // No pair... create one.
                    pair = addHalfEdge();

                    // Set border feature for both half edges
                    border(pair, true);
                    border(halfEdge, true);

                    // Set sharp feature for both half edges
                    sharp(pair, true);
                    sharp(halfEdge, true);

                    vertex(pair, vertex(next(halfEdge)));

                    TopologyToolbox::pair(pair, halfEdge);
                    TopologyToolbox::pair(halfEdge, pair);

                    holeHalfEdges.push_back(pair);
                }

                return holeHalfEdges;
            }

            void connectHoleHalfEdges(const std::vector<half_edge_index_type>& holeHalfEdges)
            {
                for (auto halfEdge : holeHalfEdges)
                {
                    auto next = TopologyToolbox::pair(halfEdge);
                    next = prev(next);
                    next = pair(next);

                    while (valid(face(next)))
                    {
                        next = prev(next);
                        next = pair(next);
                    }

                    prev(next, halfEdge);
                    TopologyToolbox::next(halfEdge, next);
                }
            }

            void createHoleFace(const std::vector<half_edge_index_type>& holeHalfEdges)
            {
                for (auto halfEdge : holeHalfEdges)
                {
                    // Already part of an hole face?
                    if (valid(face(halfEdge)))
                        continue;

                    auto face = addFace();
                    auto temp = halfEdge;

                    TopologyToolbox::halfEdge(face, temp);
                    hole(face, true);

                    do
                    {
                        TopologyToolbox::face(temp, face);
                        temp = next(temp);
                    } while (temp != halfEdge);
                }
            }

            void createHoleFaces()
            {
                auto halfEdges = createHoleHalfEdges();
                connectHoleHalfEdges(halfEdges);
                createHoleFace(halfEdges);
            }

            /*
             * Get the half edge, that is part of the given face.
             * Returns an invalid index if the face does not belong to any half edge.
             */
            auto halfEdge(edge_index_type edge, face_index_type face)
            {
                assert(valid(edge));
                assert(valid(face));

                auto a = start(edge);
                auto b = end(edge);

                assert(valid(a));
                assert(valid(b));

                if (TopologyToolbox::face(a) == face)
                    return a;

                if (TopologyToolbox::face(b) == face)
                    return b;

                return invalid_half_edge();
            }

            bool limit(vertex_index_type vertex) const
            {
                assert(valid(vertex));

                return mesh->vertexFeatures[vertex].limit;
            }

            void limit(vertex_index_type vertex, bool limit)
            {
                assert(valid(vertex));

                mesh->vertexFeatures[vertex.data].limit = limit;
            }

            static vertex_index_type invalid_vertex()
            {
                return {};
            }

            static half_edge_index_type invalid_half_edge()
            {
                return {};
            }

            static edge_index_type invalid_edge()
            {
                return {};
            }

            static face_index_type invalid_face()
            {
                return {};
            }

            std::shared_ptr<half_edge_mesh_type> getMesh()
            {
                return mesh;
            }
		protected:
			template <template <typename> class Element, typename IndexType>
			auto addElement(std::vector<Element<IndexType>>& v)
			{
				auto size = v.size();
				auto index = Index<Element<IndexType>> { static_cast<IndexType>(size) };

				v.emplace_back();

				return index;
			}

            template <template <typename> class Element, typename IndexType, typename Feature>
            auto addElement(std::vector<Element<IndexType>>& v, std::vector<Feature>& p)
			{
				auto size = v.size();
				auto index = Index<Element<IndexType>>{ static_cast<IndexType>(size) };

                // Create a new element ...
				v.emplace_back();

                // ... and create the related feature element
				p.emplace_back();

				return index;
			}

            template <template <typename> class Element, typename IndexType>
            auto indices(std::vector<Element<IndexType>>& v) const
            {
                std::vector<Index<Element<IndexType>>> result;

                for (IndexType i = 0; i < v.size(); i++)
                    result.emplace_back(i);

                return result;
            }
		protected:
			std::shared_ptr<half_edge_mesh_type> mesh;
		};
	} // namespace topology
} // namespace viscas

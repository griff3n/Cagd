#pragma once
#include <memory>
#include <QVector3D>

#include "topology.h"
#include "topologytoolbox.h"
#include "iterator.h"

namespace viscas
{
	// Simple typedefs for convenience.
	using half_edge_mesh = viscas::HalfEdgeMesh<std::size_t, QVector3D>;

	using shared_half_edge_mesh = std::shared_ptr<half_edge_mesh>;
	using topology_toolbox = viscas::topology::TopologyToolbox<half_edge_mesh>;
	using half_edge_cycle = viscas::iterator::HalfEdgeCycle<half_edge_mesh>;

} // namespace viscas

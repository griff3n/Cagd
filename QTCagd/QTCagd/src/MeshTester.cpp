#include "MeshTester.h"

MeshTester::MeshTester() {}

MeshTester::~MeshTester() {}

void MeshTester::testMesh(HalfEdgeMesh * mesh) {
	qInfo() << "Tests =============================================\n";
	//HALFEDGE TEST
	testHalfEdge(mesh);
	//FACES TEST
	testFace(mesh);
	//TEST VERTICES
	testVertex(mesh);
	qInfo() << "===================================================\n\n";
}

bool MeshTester::testHalfEdge(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->halfEdges.size(); i++) {
		if (mesh->halfEdges[i]->pair == nullptr) {
			qInfo() << "ERROR - Halfedges - Eine Halfedge hat kein pair!\n";
			return false;
		}
		if (mesh->halfEdges[i]->pair->pair == nullptr) {
			qInfo() << "ERROR - Halfedges - Eine Halfedge hat kein pair!\n";
			return false;
		}
		if (mesh->halfEdges[i]->pair->pair != mesh->halfEdges[i]) {
			qInfo() << "ERROR - Halfedges - Ein Pair ist falsch gesetzt!\n";
			return false;
		}
	}
	qInfo() << "OK - Halfedges - Alle pairs wurden konsistent gesetzt!\n";
	return true;
}

bool MeshTester::testFace(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->faces.size(); i++) {
		if (mesh->faces[i]->edge == nullptr) {
			qInfo() << "ERROR - Faces - Ein Face hat keine edge!\n";
			return false;
		}
		halfEdge *tempEdge = mesh->faces[i]->edge;
		for (int j = 0; j < mesh->faces[i]->valence; j++) {
			if (tempEdge->next == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat kein next!\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
		if (tempEdge != mesh->faces[i]->edge) {
			qInfo() << "ERROR - Faces - Die Halfedges des Faces sind nicht richtig verknüpft!\n";
			return false;
		}
		for (int j = 0; j < mesh->faces[i]->valence; j++) {
			if (tempEdge->face != mesh->faces[i]) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces ist nicht mit dem Face verbunden!\n";
				return false;
			}
			if (tempEdge->pair == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat kein pair!\n";
				return false;
			}
			if (tempEdge->pair->vert == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge hat keinen vert!\n";
				return false;
			}
			if (tempEdge->next->vert == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat keinen vert!\n";
				return false;
			}
			if (tempEdge->pair->vert != tempEdge->next->vert) {
				qInfo() << "ERROR - Faces - Ein Vertex des Faces ist nicht konsistent mit seinen Halfedges verbunden!\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
	}
	qInfo() << "OK - Faces - Alle Vertices sind konsistent mit ihren Halfedges verbunden und alle Halfedges sind konsistent mit ihrem Face verbunden!\n";
	return true;
}

bool MeshTester::testVertex(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->vertices.size(); i++) {
		if (mesh->vertices[i]->edge == nullptr) {
			qInfo() << "ERROR - Vertices - Ein Vertex hat kein edge!\n";
			return false;
		}
		halfEdge *tempEdge = mesh->vertices[i]->edge;
		for (int j = 0; j < mesh->vertices[i]->valence; j++) {
			if (tempEdge->pair == nullptr) {
				qInfo() << "ERROR - Vertices - Eine angrenzende Halfedge hat kein pair!\n";
				return false;
			}
			tempEdge = tempEdge->pair;
			if (tempEdge->next == nullptr) {
				qInfo() << "ERROR - Vertices - Eine angrenzende Halfedge hat kein next!\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
		if (tempEdge != mesh->vertices[i]->edge) {
			qInfo() << "ERROR - Vertices - Die angrenzenden Halfedges des Vertices sind nicht richtig verknüpft!\n";
			return false;
		}
	}
	qInfo() << "OK - Vertices - Alle angrenzenden Halfedges sind erreichbar!\n";
	return true;
}

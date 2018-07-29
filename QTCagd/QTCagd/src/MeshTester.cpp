#include "MeshTester.h"

MeshTester::MeshTester() {}

MeshTester::~MeshTester() {}

QString * MeshTester::testMesh(HalfEdgeMesh * mesh) {
	qInfo() << "Tests =============================================\n";
	output += "<h3><u>Tests:<u></h3>\n\n";
	//HALFEDGE TEST
	testHalfEdge(mesh);
	//FACES TEST
	testFace(mesh);
	//TEST VERTICES
	testVertex(mesh);
	qInfo() << "===================================================\n\n";
	QString * out = new QString(output);
	return out;
}

bool MeshTester::testHalfEdge(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->halfEdges.size(); i++) {
		if (mesh->halfEdges[i]->pair == nullptr) {
			qInfo() << "ERROR - Halfedges - Eine Halfedge hat kein pair!\n";
			output += "<h4>ERROR - Halfedges - Eine Halfedge hat kein pair!</h4>\n";
			return false;
		}
		if (mesh->halfEdges[i]->pair->pair == nullptr) {
			qInfo() << "ERROR - Halfedges - Eine Halfedge hat kein pair!\n";
			output += "<h4>ERROR - Halfedges - Eine Halfedge hat kein pair!</h4>\n";
			return false;
		}
		if (mesh->halfEdges[i]->pair->pair != mesh->halfEdges[i]) {
			qInfo() << "ERROR - Halfedges - Ein Pair ist falsch gesetzt!\n";
			output += "<h4>ERROR - Halfedges - Ein Pair ist falsch gesetzt!</h4>\n";
			return false;
		}
	}
	qInfo() << "OK - Halfedges - Alle pairs wurden konsistent gesetzt!\n";
	output += "<h4>OK - Halfedges - Alle pairs wurden konsistent gesetzt!</h4>\n";
	return true;
}

bool MeshTester::testFace(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->faces.size(); i++) {
		if (mesh->faces[i]->edge == nullptr) {
			qInfo() << "ERROR - Faces - Ein Face hat keine edge!\n";
			output += "<h4>ERROR - Faces - Ein Face hat keine edge!</h4>\n";
			return false;
		}
		halfEdge *tempEdge = mesh->faces[i]->edge;
		for (int j = 0; j < mesh->faces[i]->valence; j++) {
			if (tempEdge->next == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat kein next!\n";
				output += "<h4>ERROR - Faces - Eine Halfedge des Faces hat kein next!</h4>\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
		if (tempEdge != mesh->faces[i]->edge) {
			qInfo() << "ERROR - Faces - Die Halfedges des Faces sind nicht richtig verknüpft!\n";
			output += "<h4>ERROR - Faces - Die Halfedges des Faces sind nicht richtig verknüpft!</h4>\n";
			return false;
		}
		for (int j = 0; j < mesh->faces[i]->valence; j++) {
			if (tempEdge->face != mesh->faces[i]) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces ist nicht mit dem Face verbunden!\n";
				output += "<h4>ERROR - Faces - Eine Halfedge des Faces ist nicht mit dem Face verbunden!</h4>\n";
				return false;
			}
			if (tempEdge->pair == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat kein pair!\n";
				output += "<h4>ERROR - Faces - Eine Halfedge des Faces hat kein pair!</h4>\n";
				return false;
			}
			if (tempEdge->pair->vert == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge hat keinen vert!\n";
				output += "<h4>ERROR - Faces - Eine Halfedge hat keinen vert!</h4>\n";
				return false;
			}
			if (tempEdge->next->vert == nullptr) {
				qInfo() << "ERROR - Faces - Eine Halfedge des Faces hat keinen vert!\n";
				output += "<h4>ERROR - Faces - Eine Halfedge des Faces hat keinen vert!</h4>\n";
				return false;
			}
			if (tempEdge->pair->vert != tempEdge->next->vert) {
				qInfo() << "ERROR - Faces - Ein Vertex des Faces ist nicht konsistent mit seinen Halfedges verbunden!\n";
				output += "<h4>ERROR - Faces - Ein Vertex des Faces ist nicht konsistent mit seinen Halfedges verbunden!</h4>\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
	}
	qInfo() << "OK - Faces - Alle Vertices sind konsistent mit ihren Halfedges verbunden und alle Halfedges sind konsistent mit ihrem Face verbunden!\n";
	output += "<h4>OK - Faces - Alle Vertices sind konsistent mit ihren Halfedges verbunden und alle Halfedges sind konsistent mit ihrem Face verbunden!</h4>\n";
	return true;
}

bool MeshTester::testVertex(HalfEdgeMesh * mesh) {
	for (int i = 0; i < mesh->vertices.size(); i++) {
		if (mesh->vertices[i]->edge == nullptr) {
			qInfo() << "ERROR - Vertices - Ein Vertex hat kein edge!\n";
			output += "<h4>ERROR - Vertices - Ein Vertex hat kein edge!</h4>\n";
			return false;
		}
		halfEdge *tempEdge = mesh->vertices[i]->edge;
		for (int j = 0; j < mesh->vertices[i]->valence; j++) {
			if (tempEdge->pair == nullptr) {
				qInfo() << "ERROR - Vertices - Eine angrenzende Halfedge hat kein pair!\n";
				output += "<h4>ERROR - Vertices - Eine angrenzende Halfedge hat kein pair!</h4>\n";
				return false;
			}
			tempEdge = tempEdge->pair;
			if (tempEdge->next == nullptr) {
				qInfo() << "ERROR - Vertices - Eine angrenzende Halfedge hat kein next!\n";
				output += "<h4>ERROR - Vertices - Eine angrenzende Halfedge hat kein next!</h4>\n";
				return false;
			}
			tempEdge = tempEdge->next;
		}
		if (tempEdge != mesh->vertices[i]->edge) {
			qInfo() << "ERROR - Vertices - Die angrenzenden Halfedges des Vertices sind nicht richtig verknüpft!\n";
			output += "<h4>ERROR - Vertices - Die angrenzenden Halfedges des Vertices sind nicht richtig verknüpft!</h4>\n";
			return false;
		}
	}
	qInfo() << "OK - Vertices - Alle angrenzenden Halfedges sind erreichbar!\n";
	output += "<h4>OK - Vertices - Alle angrenzenden Halfedges sind erreichbar!</h4>\n";
	return true;
}


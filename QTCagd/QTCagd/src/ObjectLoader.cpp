#include "ObjectLoader.h"

 
HalfEdgeMesh* loadOBJreg(std::string path) {

	// Lists of all outgoing Edges of every Vertex to speed up the Process
	std::vector<std::vector<halfEdge*>> acceleration;
	float bbox[6];
	bbox[0] = std::numeric_limits<float>::max();
	bbox[1] = -std::numeric_limits<float>::max();
	bbox[2] = std::numeric_limits<float>::max();
	bbox[3] = -std::numeric_limits<float>::max();
	bbox[4] = std::numeric_limits<float>::max();
	bbox[5] = -std::numeric_limits<float>::max();
	float intitalSize = 1.0;

	std::string line;
	std::string name;
	std::regex skip("^# | ^\\s*$"); // Comments and empty Lines
	std::regex reg("[\\w.-]+"); // Words or Numbers
	std::ifstream myfile(path);
	HalfEdgeMesh* mesh = new HalfEdgeMesh();

	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (std::regex_search(line, skip))
			{// Skip unimportant lines and continue
				continue;
			}
			std::vector<std::smatch> results;
			std::sregex_iterator next(line.begin(), line.end(), reg);
			std::sregex_iterator end;
			while (next != end)
			{
				std::smatch match = *next;
				results.push_back(match);
				next++;
			}
			if (results[0].str() == "v")
			{
				try {
					float x = std::stof(results[1].str());
					float y = std::stof(results[2].str());
					float z = std::stof(results[3].str());
					graphicVertex* newVert = new graphicVertex(QVector4D(x, y, z, 1.0f));
					mesh->vertices.push_back(newVert);

					if (newVert->location.x() < bbox[0]) bbox[0] = newVert->location.x();
					if (newVert->location.x() > bbox[1]) bbox[1] = newVert->location.x();
					if (newVert->location.y() < bbox[2]) bbox[2] = newVert->location.y();
					if (newVert->location.y() > bbox[3]) bbox[3] = newVert->location.y();
					if (newVert->location.z() < bbox[4]) bbox[4] = newVert->location.z();
					if (newVert->location.z() > bbox[5]) bbox[5] = newVert->location.z();
				}
				catch(const std::invalid_argument& ia){
					qInfo() << "Invalid argument: " << ia.what() << '\n';
				}
			}
			else if (results[0].str() == "f")
			{
				if (acceleration.size() == 0) acceleration.resize(mesh->vertices.size()); //Only once after reading all vertices
				
				std::vector<int> indices; // Indices of Vertices of the current Face
				try {
					for (int i = 1; i < results.size(); i++) {
						indices.push_back(stoi(results[i].str()));
					}
				}
				catch (const std::invalid_argument& ia) {
					qInfo() << "Invalid argument: " << ia.what() << '\n';
				}
				graphicFace * face = new graphicFace;
				halfEdge *previous = nullptr;
				halfEdge *first = nullptr;
				for (int j = 0; j < indices.size(); j++) {
					halfEdge *edge = new halfEdge;
					edge->face = face;
					graphicVertex *vert = mesh->vertices[indices[j] - 1];
					if (vert->edge == nullptr) {
						vert->edge = edge;
					}
					acceleration[indices[j] - 1].push_back(edge);
					edge->vert = vert;
					if (previous == nullptr) {
						first = edge;
					}
					else {
						previous->next = edge;
					}
					previous = edge;
					mesh->halfEdges.push_back(edge);
				}
				previous->next = first;
				face->edge = first;
				face->valence = indices.size(); //Face valences
				mesh->faces.push_back(face);
				// setting Pairs
				for (int k = 0; k < indices.size(); k++) {
					graphicVertex *start = first->vert;
					graphicVertex *end = first->next->vert;
					int index = indices[(k + 1) % indices.size()] - 1;
					for (int n = 0; n < acceleration[index].size(); n++) {
						halfEdge* pairCheck = acceleration[index][n];
						if (pairCheck->next != nullptr && pairCheck->next->vert == start) {							
								first->pair = pairCheck;
								pairCheck->pair = first;
								break;
						}
					}
					first = first->next;
				}
			}
			else
			{
				//something else
			}
		}
		//Vertice valences
		for (int i = 0; i < mesh->vertices.size(); i++) {
			mesh->vertices[i]->valence = acceleration[i].size();
		}
		myfile.close();

		//Using bbox to scale and translate the model
		float xlength = bbox[1] - bbox[0];
		float ylength = bbox[3] - bbox[2];
		float zlength = bbox[5] - bbox[4];

		float scale;
		if (xlength >= ylength && xlength >= zlength) {
			scale = 2.0f*intitalSize / xlength;
		}
		else if (ylength >= xlength && ylength >= zlength) {
			scale = 2.0f*intitalSize / ylength;
		}
		else {
			scale = 2.0f*intitalSize / zlength;
		}
		QVector3D invMiddlepoint = QVector3D(-(xlength / 2 + bbox[0])*scale, -(ylength / 2 + bbox[2])*scale, -(zlength / 2 + bbox[4])*scale);
		mesh->model.translate(invMiddlepoint);
		mesh->model.scale(scale);

		mesh->scale = scale;

		for (int i = 0; i < mesh->halfEdges.size(); i++) {
			halfEdge * he = mesh->halfEdges.at(i);
			if (he->pair == nullptr) {
				graphicFace * hole = new graphicFace;
				hole->isHole = true;
				halfEdge *holeEdge = new halfEdge;
				he->pair = holeEdge;
				holeEdge->pair = he;
				holeEdge->vert = he->next->vert;
				holeEdge->face = hole;
				hole->edge = holeEdge;
				mesh->faces.push_back(hole);
				halfEdge * current = he;
				halfEdge *lastHoleEdge = holeEdge;
				while (current -> next != he) {
					current = current->next;
					if (current->pair == nullptr) {
						halfEdge *newHoleEdge = new halfEdge;
						current->pair = newHoleEdge;
						newHoleEdge->pair = current;
						newHoleEdge->vert = current->next->vert;
						newHoleEdge->face = hole;
						newHoleEdge->next = lastHoleEdge;
						lastHoleEdge = newHoleEdge;
						mesh->halfEdges.push_back(newHoleEdge);
					}
					else current = current->pair;
				}
				holeEdge->next = lastHoleEdge;
				mesh->halfEdges.push_back(holeEdge);
				//Hole Valence
				int v = 1;
				current = holeEdge;
				while (current->next != holeEdge) {
					v++;
					current = current->next;
				}
				hole->valence = v;
			}
		}

		return mesh;
	}
	else qInfo() << "Unable to open file\n";
	return nullptr;
}
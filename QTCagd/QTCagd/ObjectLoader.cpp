#include "ObjectLoader.h"

std::vector<std::vector<HE_edge*>> acceleration;
std::vector<glm::vec2*> HEtextureVertices;
std::vector<glm::vec3*> HEVertexNormals;

bool loadOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<HE_vert*> &out_HEvertices, std::vector<HE_face*> &out_HEfaces, std::vector<HE_edge*> &out_HEedges, std::vector<float> &bbox) {
	std::string line;
	std::string name;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line == "" || line[0] == '#') {// Skip unimportant lines and continue
				continue;
			}
			if (line[0] == 'v') {
				std::string delimiter = " ";
				line.erase(0, 1 + delimiter.length());
				size_t pos = 0;
				std::string token;
				std::vector<float> coordinates;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					coordinates.push_back(std::stof(token));
					line.erase(0, pos + delimiter.length());
				}
				coordinates.push_back(std::stof(line));
				if (coordinates.size() != 3) {
					return false; //obj file is corrupt
				}
				else {
					glm::vec3 newVertice = glm::vec3({ coordinates[0], coordinates[1], coordinates[2] });
					//BEGIN HE datastructure =================================
					HE_vert * vert = new HE_vert;
					vert->x = coordinates[0];
					vert->y = coordinates[1];
					vert->z = coordinates[2];
					vert->edge = nullptr;
					out_HEvertices.push_back(vert);
					//END HE datastructure ===================================
					//Get BBox Coordinates
					if (bbox[0] == NULL) { //first vertice
						bbox[0] = vert->x;
						bbox[1] = vert->x;
						bbox[2] = vert->y;
						bbox[3] = vert->y;
						bbox[4] = vert->z;
						bbox[5] = vert->z;
					}
					else {
						if (vert->x < bbox[0]) bbox[0] = vert->x;
						if (vert->x > bbox[1]) bbox[1] = vert->x;
						if (vert->y < bbox[2]) bbox[2] = vert->y;
						if (vert->y > bbox[3]) bbox[3] = vert->y;
						if (vert->z < bbox[4]) bbox[4] = vert->z;
						if (vert->z > bbox[5]) bbox[5] = vert->z;
					}
					out_vertices.push_back(newVertice);
				}
			}
			else if (line[0] == 'f') {
				//BEGIN HE datastructure =====================================
				if (acceleration.size() == 0) acceleration.resize(out_HEvertices.size()); //Only once after reading all vertices
																						  //END HE datastructure =======================================
				std::string delimiter = " ";
				if (line[2] == ' ') {
					line.erase(0, 2 + delimiter.length());
				}
				else {
					line.erase(0, 1 + delimiter.length());
				}
				size_t pos = 0;
				std::string token;
				std::vector<int> indices;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					indices.push_back(std::stoi(token));
					line.erase(0, pos + delimiter.length());
				}
				indices.push_back(std::stoi(line));
				if (indices.size() < 3) {
					return false; //obj file is corrupt
				}
				for (int i = 2; i < indices.size(); i++) {
					out_indices.push_back(indices[0] - 1);
					out_indices.push_back(indices[i - 1] - 1);
					out_indices.push_back(indices[i] - 1);
				}
				//BEGIN HE datastructure =====================================
				HE_face * face = new HE_face;
				HE_edge *previous = nullptr;
				HE_edge *first = nullptr;
				for (int j = 0; j < indices.size(); j++) {
					HE_edge *edge = new HE_edge;
					edge->face = face;
					HE_vert *vert = out_HEvertices[indices[j] - 1];
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
					out_HEedges.push_back(edge);
				}
				previous->next = first;
				face->edge = first;
				//Face valences
				face->valence = indices.size();
				out_HEfaces.push_back(face);
				for (int k = 0; k < indices.size(); k++) {
					HE_vert *start = first->vert;
					HE_vert *end = first->next->vert;
					int index = indices[(k + 1) % indices.size()] - 1;
					for (int n = 0; n < acceleration[index].size(); n++) {
						if (acceleration[index][n]->next != nullptr) {
							if (acceleration[index][n]->next->vert == start) {
								first->pair = acceleration[index][n];
								acceleration[index][n]->pair = first;
							}
						}
					}
					first = first->next;
				}
				//END HE datastructure =======================================
			}
		}
		//Vertice valences
		for (int i = 0; i < out_HEvertices.size(); i++) {
			out_HEvertices[i]->valence = acceleration[i].size();
		}
		myfile.close();
		return true;
	}

	else std::cout << "Unable to open file";
	return false;
}

bool loadMayaOBJ(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<HE_vert*> &out_HEvertices, std::vector<HE_face*> &out_HEfaces, std::vector<HE_edge*> &out_HEedges, std::vector<float> &bbox) {
	std::string line;
	std::string name;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line == "" || line[0] == '#' || line[0] == 's' || line[0] == 'g') {// Skip unimportant lines and continue
				continue;
			}
			if (line[0] == 'v' && line[1] == 't') {
				std::string delimiter = " ";
				line.erase(0, 2 + delimiter.length());
				size_t pos = 0;
				std::string token;
				std::vector<float> textureCoordinates;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					textureCoordinates.push_back(std::stof(token));
					line.erase(0, pos + delimiter.length());
				}
				textureCoordinates.push_back(std::stof(line));
				if (textureCoordinates.size() != 2) {
					return false; //obj file is corrupt
				}
				else {
					//BEGIN HE datastructure =================================
					glm::vec2 *vert = new glm::vec2(textureCoordinates[0], textureCoordinates[1]);
					HEtextureVertices.push_back(vert);
					//END HE datastructure ===================================
				}
			}
			else if (line[0] == 'v' && line[1] == 'n') {
				std::string delimiter = " ";
				line.erase(0, 2 + delimiter.length());
				size_t pos = 0;
				std::string token;
				std::vector<float> normalCoordinates;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					normalCoordinates.push_back(std::stof(token));
					line.erase(0, pos + delimiter.length());
				}
				normalCoordinates.push_back(std::stof(line));
				if (normalCoordinates.size() != 3) {
					return false; //obj file is corrupt
				}
				else {
					//BEGIN HE datastructure =================================
					glm::vec3 *vert = new glm::vec3(normalCoordinates[0], normalCoordinates[1], normalCoordinates[2]);
					HEVertexNormals.push_back(vert);
					//END HE datastructure ===================================
				}
			}
			else if (line[0] == 'v') {
				std::string delimiter = " ";
				line.erase(0, 1 + delimiter.length());
				size_t pos = 0;
				std::string token;
				std::vector<float> coordinates;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					coordinates.push_back(std::stof(token));
					line.erase(0, pos + delimiter.length());
				}
				coordinates.push_back(std::stof(line));
				if (coordinates.size() != 3) {
					return false; //obj file is corrupt
				}
				else {
					glm::vec3 newVertice = glm::vec3({ coordinates[0], coordinates[1], coordinates[2] });
					//BEGIN HE datastructure =================================
					HE_vert * vert = new HE_vert;
					vert->x = coordinates[0];
					vert->y = coordinates[1];
					vert->z = coordinates[2];
					vert->edge = nullptr;
					vert->texture = nullptr;
					vert->normal = nullptr;
					out_HEvertices.push_back(vert);
					//END HE datastructure ===================================
					//Get BBox Coordinates
					if (bbox[0] == NULL) { //first vertice
						bbox[0] = vert->x;
						bbox[1] = vert->x;
						bbox[2] = vert->y;
						bbox[3] = vert->y;
						bbox[4] = vert->z;
						bbox[5] = vert->z;
					}
					else {
						if (vert->x < bbox[0]) bbox[0] = vert->x;
						if (vert->x > bbox[1]) bbox[1] = vert->x;
						if (vert->y < bbox[2]) bbox[2] = vert->y;
						if (vert->y > bbox[3]) bbox[3] = vert->y;
						if (vert->z < bbox[4]) bbox[4] = vert->z;
						if (vert->z > bbox[5]) bbox[5] = vert->z;
					}
					out_vertices.push_back(newVertice);
				}
			}
			else if (line[0] == 'f') {
				//BEGIN HE datastructure =====================================
				if (acceleration.size() == 0) acceleration.resize(out_HEvertices.size()); //Only once after reading all vertices
																						  //END HE datastructure =======================================
				std::string delimiter = " ";
				if (line[2] == ' ') {
					line.erase(0, 2 + delimiter.length());
				}
				else {
					line.erase(0, 1 + delimiter.length());
				}
				size_t pos = 0;
				std::string token;
				std::vector<int> indices;
				std::vector<int> vtIndices;
				std::vector<int> vnIndices;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					size_t pos2 = 0;
					std::string token2;
					pos2 = token.find('/');
					token2 = token.substr(0, pos2);
					indices.push_back(std::stoi(token2));
					token.erase(0, pos2 + delimiter.length());
					pos2 = token.find('/');
					token2 = token.substr(0, pos2);
					vtIndices.push_back(std::stoi(token2));
					token.erase(0, pos2 + delimiter.length());
					vnIndices.push_back(std::stoi(token));
					line.erase(0, pos + delimiter.length());
				}
				size_t pos2 = 0;
				std::string token2;
				pos2 = line.find('/');
				token2 = line.substr(0, pos2);
				indices.push_back(std::stoi(token2));
				line.erase(0, pos2 + delimiter.length());
				pos2 = line.find('/');
				token2 = line.substr(0, pos2);
				vtIndices.push_back(std::stoi(token2));
				line.erase(0, pos2 + delimiter.length());
				vnIndices.push_back(std::stoi(line));
				if (indices.size() < 3) {
					return false; //obj file is corrupt
				}
				for (int i = 2; i < indices.size(); i++) {
					out_indices.push_back(indices[0] - 1);
					out_indices.push_back(indices[i - 1] - 1);
					out_indices.push_back(indices[i] - 1);
				}
				//BEGIN HE datastructure =====================================
				HE_face * face = new HE_face;
				HE_edge *previous = nullptr;
				HE_edge *first = nullptr;
				for (int j = 0; j < indices.size(); j++) {
					HE_edge *edge = new HE_edge;
					edge->face = face;
					HE_vert *vert = out_HEvertices[indices[j] - 1];
					if (vert->edge == nullptr) {
						vert->edge = edge;
					}
					if (vert->texture == nullptr) {
						vert->texture = HEtextureVertices[vtIndices[j] - 1];
					}
					if (vert->normal == nullptr) {
						vert->normal = HEVertexNormals[vnIndices[j] - 1];
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
					out_HEedges.push_back(edge);
				}
				previous->next = first;
				face->edge = first;
				//Face valences
				face->valence = indices.size();
				out_HEfaces.push_back(face);
				for (int k = 0; k < indices.size(); k++) {
					HE_vert *start = first->vert;
					HE_vert *end = first->next->vert;
					int index = indices[(k + 1) % indices.size()] - 1;
					for (int n = 0; n < acceleration[index].size(); n++) {
						if (acceleration[index][n]->next != nullptr) {
							if (acceleration[index][n]->next->vert == start) {
								first->pair = acceleration[index][n];
								acceleration[index][n]->pair = first;
							}
						}
					}
					first = first->next;
				}
				//END HE datastructure =======================================
			}
		}
		//Vertice valences
		for (int i = 0; i < out_HEvertices.size(); i++) {
			out_HEvertices[i]->valence = acceleration[i].size();
		}
		std::cout << "v " << out_HEvertices.size() << ", vt " << HEtextureVertices.size() << ", vn " << HEVertexNormals.size() << std::endl;
		myfile.close();
		return true;
	}

	else std::cout << "Unable to open file";
	return false;
}

bool loadBezier(const char * path, std::vector<glm::vec3> & out_vertices, std::vector<GLuint> &out_indices, std::vector<float> &bbox) {
	std::string line;
	std::ifstream myfile(path);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			if (line == "" || line[0] == '#') {// Skip unimportant lines and continue
				continue;
			}
			if (line == "cstype bezier") {
				int degree;
				std::vector<int> indices;
				while (getline(myfile, line)) {
					if (line == "end") {
						break;
					}
					else if (line[0] == 'd' && line[1] == 'e' && line[2] == 'g') {
						std::string delimiter = " ";
						line.erase(0, 3 + delimiter.length());
						degree = std::stoi(line);
					}
					else if (line[0] == 'c' && line[1] == 'u' && line[2] == 'r' && line[3] == 'v') {
						std::string delimiter = " ";
						line.erase(0, 4 + delimiter.length());
						size_t pos = 0;
						std::string token;
						while ((pos = line.find(delimiter)) != std::string::npos) {
							token = line.substr(0, pos);
							indices.push_back(std::stoi(token));
							line.erase(0, pos + delimiter.length());
						}
						if (line != "") indices.push_back(std::stoi(line));
					}
				}
				if (indices.size() != degree + 1) {
					return false;
				}
				else {
					for (int i = 0; i < indices.size(); i++) {
						out_indices.push_back(indices[i] - 1);
					}
				}
			}
			if (line[0] == 'v') {
				std::string delimiter = " ";
				line.erase(0, 1 + delimiter.length());
				size_t pos = 0;
				std::string token;
				std::vector<float> coordinates;
				while ((pos = line.find(delimiter)) != std::string::npos) {
					token = line.substr(0, pos);
					coordinates.push_back(std::stof(token));
					line.erase(0, pos + delimiter.length());
					while (line[0] == ' ') {
						line.erase(0, delimiter.length());
					}
				}
				if (line != "")coordinates.push_back(std::stof(line));
				if (coordinates.size() != 3) {
					return false; //obj file is corrupt
				}
				else {
					glm::vec3 newVertice = glm::vec3({ coordinates[0], coordinates[1], coordinates[2] });
					//Get BBox Coordinates
					if (bbox[0] == NULL) { //first vertice
						bbox[0] = newVertice.x;
						bbox[1] = newVertice.x;
						bbox[2] = newVertice.y;
						bbox[3] = newVertice.y;
						bbox[4] = newVertice.z;
						bbox[5] = newVertice.z;
					}
					else {
						if (newVertice.x < bbox[0]) bbox[0] = newVertice.x;
						if (newVertice.x > bbox[1]) bbox[1] = newVertice.x;
						if (newVertice.y < bbox[2]) bbox[2] = newVertice.y;
						if (newVertice.y > bbox[3]) bbox[3] = newVertice.y;
						if (newVertice.z < bbox[4]) bbox[4] = newVertice.z;
						if (newVertice.z > bbox[5]) bbox[5] = newVertice.z;
					}
					out_vertices.push_back(newVertice);
				}
			}
		}
		myfile.close();
		return true;
	}

	else std::cout << "Unable to open file";
	return false;
}

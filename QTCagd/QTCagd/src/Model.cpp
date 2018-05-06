#include "Model.h"

// Create a Model from obj file.
Model::Model(char * path, float size, glm::vec3 color, glm::vec3 position, bool maya)
{
	Model::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");
	initObject(path, color, size, maya);
	translate(position);
}

Model::~Model() {
	delete boundingBox;
	delete helpNormals;
	delete normals;
	for (HE_vert* vertex : heVertices) {
		delete vertex;
	}
	for (HE_edge* edge : heEdges) {
		delete edge;
	}
	for (HE_face* face : heFaces) {
		delete face;
	}
}

void Model::render(const glm::mat4 & view, const glm::mat4 & projection, const bool bbox, const bool hnorms, const bool norms)
{
	if (bbox) boundingBox->render(view, projection);
	if (hnorms) helpNormals->render(view, projection);
	if (norms) normals->render(view, projection);
	// Create mvp.
	glm::mat4 mvp = projection * view * model;

	// Bind the shader program and set uniform(s).
	programSimple.use();
	programSimple.setUniform("mvp", mvp);

	// Bind vertex array object.
	glBindVertexArray(customModel.vao);
	glDrawElements(GL_TRIANGLES, customModel.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Model::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
	boundingBox->translate(vector);
	helpNormals->translate(vector);
	normals->translate(vector);
}

void Model::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
	boundingBox->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	helpNormals->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	normals->rotateAroundAxis(rot_speed, axis, positionOfAxis);
}

void Model::faceDirection(glm::vec3 direction) {
	//model *= glm::orientation(direction, glm::vec3(0.0f, 0.0f, 1.0f));

	if (direction.x == 0.0f && direction.z == 0.0f) {
		if (direction.y < 0) rotateAroundAxis(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
		return;
	}
	float angleY = glm::acos(glm::dot(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
	rotateAroundAxis(angleY, axis, glm::vec3(0.0f));

}

void Model::reset() {
	glm::mat4 unitMatrix;
	model = glm::scale(unitMatrix, glm::vec3(scale));
	boundingBox->model = glm::scale(unitMatrix, glm::vec3(scale));
	helpNormals->model = glm::scale(unitMatrix, glm::vec3(1.0));
	normals->model = glm::scale(unitMatrix, glm::vec3(1.0));
}

void Model::runTests() {
	std::cout << "Tests =============================================" << std::endl;
	//HALFEDGE TEST
	bool error = false;
	for (int i = 0; i < heEdges.size(); i++) {
		if (heEdges[i]->pair == nullptr) {
			error = true;
			std::cout << "ERROR - Halfedges - Eine Halfedge hat kein pair!" << std::endl;
			break;
		}
		else if (heEdges[i]->pair->pair == nullptr) {
			error = true;
			std::cout << "ERROR - Halfedges - Eine Halfedge hat kein pair!" << std::endl;
			break;
		}
		else if (heEdges[i]->pair->pair != heEdges[i]) {
			error = true;
			std::cout << "ERROR - Halfedges - Ein Pair ist falsch gesetzt!" << std::endl;
			break;
		}
	}
	if (!error) std::cout << "OK - Halfedges - Alle pairs wurden konsistent gesetzt!" << std::endl;
	//FACES TEST
	error = false;
	for (int i = 0; i < heFaces.size(); i++) {
		if (heFaces[i]->edge == nullptr) {
			error = true;
			std::cout << "ERROR - Faces - Ein Face hat keine edge!" << std::endl;
			break;
		}
		else {
			HE_edge *tempEdge = heFaces[i]->edge;
			for (int j = 0; j < heFaces[i]->valence; j++) {
				if (tempEdge->next == nullptr) {
					error = true;
					std::cout << "ERROR - Faces - Eine Halfedge des Faces hat kein next!" << std::endl;
					break;
				}
				tempEdge = tempEdge->next;
			}
			if (error) break;
			if (tempEdge != heFaces[i]->edge) {
				error = true;
				std::cout << "ERROR - Faces - Die Halfedges des Faces sind nicht richtig verknüpft!" << std::endl;
				break;
			}
			for (int j = 0; j < heFaces[i]->valence; j++) {
				if (tempEdge->face != heFaces[i]) {
					error = true;
					std::cout << "ERROR - Faces - Eine Halfedge des Faces ist nicht mit dem Face verbunden!" << std::endl;
					break;
				}
				if (tempEdge->pair == nullptr) {
					error = true;
					std::cout << "ERROR - Faces - Eine Halfedge des Faces hat kein pair!" << std::endl;
					break;
				}
				else if (tempEdge->pair->vert == nullptr) {
					error = true;
					std::cout << "ERROR - Faces - Eine Halfedge hat keinen vert!" << std::endl;
					break;
				}
				else if (tempEdge->next->vert == nullptr) {
					error = true;
					std::cout << "ERROR - Faces - Eine Halfedge des Faces hat keinen vert!" << std::endl;
					break;
				}
				else if (tempEdge->pair->vert != tempEdge->next->vert) {
					error = true;
					std::cout << "ERROR - Faces - Ein Vertex des Faces ist nicht konsistent mit seinen Halfedges verbunden!" << std::endl;
					break;
				}
				tempEdge = tempEdge->next;
			}
			if (error) break;
		}
	}
	if (!error) std::cout << "OK - Faces - Alle Vertices sind konsistent mit ihren Halfedges verbunden und alle Halfedges sind konsistent mit ihrem Face verbunden!" << std::endl;
	//TEST VERTICES
	error = false;
	for (int i = 0; i < heVertices.size(); i++) {
		if (heVertices[i]->edge == nullptr) {
			error = true;
			std::cout << "ERROR - Vertices - Ein Vertex hat kein edge!" << std::endl;
			break;
		}
		else {
			HE_edge *tempEdge = heVertices[i]->edge;
			for (int j = 0; j < heVertices[i]->valence; j++) {
				if (tempEdge->pair == nullptr) {
					error = true;
					std::cout << "ERROR - Vertices - Eine angrenzende Halfedge hat kein pair!" << std::endl;
					break;
				}
				tempEdge = tempEdge->pair;
				if (tempEdge->next == nullptr) {
					error = true;
					std::cout << "ERROR - Vertices - Eine angrenzende Halfedge hat kein next!" << std::endl;
					break;
				}
				tempEdge = tempEdge->next;
			}
			if (error) break;
			if (tempEdge != heVertices[i]->edge) {
				error = true;
				std::cout << "ERROR - Vertices - Die angrenzenden Halfedges des Vertices sind nicht richtig verknüpft!" << std::endl;
				break;
			}
		}
	}
	if (!error) std::cout << "OK - Vertices - Alle angrenzenden Halfedges sind erreichbar!" << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << std::endl;
}

void Model::showStatistics() {
	std::cout << "Statistik =========================================" << std::endl;
	std::cout << "Vertices: " << heVertices.size() << std::endl;
	std::vector<int> vertexValences;
	for (int i = 0; i < heVertices.size(); i++) {
		if (heVertices[i]->valence + 1 > vertexValences.size()) vertexValences.resize(heVertices[i]->valence + 1);
		vertexValences[heVertices[i]->valence] ++;
	}
	for (int i = 0; i < vertexValences.size(); i++) {
		if (vertexValences[i] > 0) {
			std::cout << "Vertex-Valenz " << i << ": " << vertexValences[i] << std::endl;
		}
	}
	std::cout << "Faces: " << heFaces.size() << std::endl;
	std::vector<int> faceValences;
	for (int i = 0; i < heFaces.size(); i++) {
		if (heFaces[i]->valence + 1 > faceValences.size()) faceValences.resize(heFaces[i]->valence + 1);
		faceValences[heFaces[i]->valence] ++;
	}
	for (int i = 0; i < faceValences.size(); i++) {
		if (faceValences[i] > 0) {
			std::cout << "Face-Valenz " << i << ": " << faceValences[i] << std::endl;
		}
	}
	std::cout << "Halfedges: " << heEdges.size() << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << std::endl;
}

void Model::initObject(const char * path, const glm::vec3 & color, const float size, bool maya)
{
	// Build the model.
	std::vector<glm::vec3> positions;
	std::vector<GLuint> indices;
	std::vector<float> bbox;
	bbox.resize(6);
	if (maya) {
		if (!loadMayaOBJ(path, positions, indices, heVertices, heFaces, heEdges, bbox)) {
			positions.push_back(glm::vec3({ 0.0f, 1.0f, 0.0f }));
			positions.push_back(glm::vec3({ 0.0f, -1.0f, 0.0f }));
			positions.push_back(glm::vec3({ 1.0f, 0.0f, 0.0f }));
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
		}
	}
	else {
		if (!loadOBJ(path, positions, indices, heVertices, heFaces, heEdges, bbox)) {
			positions.push_back(glm::vec3({ 0.0f, 1.0f, 0.0f }));
			positions.push_back(glm::vec3({ 0.0f, -1.0f, 0.0f }));
			positions.push_back(glm::vec3({ 1.0f, 0.0f, 0.0f }));
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(2);
		}
	}

	boundingBox = new BBox(bbox, glm::vec3(1.0f, 1.0f, 1.0f));
	if (maya) {
		// Build the Normals.
		vNormals.resize(positions.size());
		for (int i = 0; i < heVertices.size(); i++) {
			vNormals[i] = *heVertices[i]->normal;
		}
	}
	else {
		// Build the helpNormals and Normals.
		vHnormals.resize(positions.size());
		vNormals.resize(positions.size());
		for (int i = 0; i < heVertices.size(); i++) {
			glm::vec3 origin = { heVertices[i]->x, heVertices[i]->y, heVertices[i]->z };
			HE_edge *tempEdge = heVertices[i]->edge;
			std::vector<glm::vec3> hNormals;
			for (int j = 0; j < heVertices[i]->valence; j++) {
				glm::vec3 dir1 = { tempEdge->pair->vert->x, tempEdge->pair->vert->y, tempEdge->pair->vert->z };
				dir1 = dir1 - origin;
				glm::vec3 dir2 = { tempEdge->pair->next->pair->vert->x, tempEdge->pair->next->pair->vert->y, tempEdge->pair->next->pair->vert->z };
				dir2 = dir2 - origin;
				glm::vec3 helpNormal = glm::cross(dir2, dir1);
				hNormals.push_back(helpNormal);
				helpNormal = glm::normalize(helpNormal);
				vHnormals[i].push_back(helpNormal);
				tempEdge = tempEdge->pair->next;
			}
			glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
			for (int j = 0; j < hNormals.size(); j++) {
				normal += hNormals[j];
			}
			normal = glm::normalize(normal);
			vNormals[i] = normal;
		}
	}

	std::vector<glm::vec3> colors;

	for (std::size_t i = 0; i < positions.size(); i++)
		//colors.push_back(color);
	{
		if (i % 2 == 0)colors.push_back(color);
		else colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	}

	GLuint programId = programSimple.getHandle();
	GLuint pos;

	// Vertex array object.
	glGenVertexArrays(1, &customModel.vao);
	glBindVertexArray(customModel.vao);

	// Position buffer.
	glGenBuffers(1, &customModel.positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, customModel.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Color buffer.
	glGenBuffers(1, &customModel.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, customModel.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index buffer.
	customModel.indexCount = (GLuint)indices.size();

	glGenBuffers(1, &customModel.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, customModel.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, customModel.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	float xlength = bbox[1] - bbox[0];
	float ylength = bbox[3] - bbox[2];
	float zlength = bbox[5] - bbox[4];

	float scale;
	if (xlength >= ylength && xlength >= zlength) {
		scale = 2.0f*size / xlength;
	}
	else if (ylength >= xlength && ylength >= zlength) {
		scale = 2.0f*size / ylength;
	}
	else {
		scale = 2.0f*size / zlength;
	}
	this->scale = scale;
	model = glm::scale(model, glm::vec3(scale));
	boundingBox->model = glm::scale(boundingBox->model, glm::vec3(scale));
	std::vector<glm::vec3> hnPositions;
	std::vector<glm::vec3> nPositions;
	for (int i = 0; i < vHnormals.size(); i++) {
		glm::vec3 origin = { heVertices[i]->x * scale, heVertices[i]->y * scale, heVertices[i]->z * scale };
		for (int j = 0; j < vHnormals[i].size(); j++) {
			glm::vec3 helpNormal = vHnormals[i][j];
			//helpNormal *= 0.2f; //GROESSE der NORMALEN
			helpNormal += origin;
			hnPositions.push_back(origin);
			hnPositions.push_back(helpNormal);
			//std::cout << "origin " << origin.x << " " << origin.y << " " << origin.z << ", helpNormal " << helpNormal.x << " " << helpNormal.y << " " << helpNormal.z << std::endl;
		}
		glm::vec3 normal = vNormals[i];
		//normal *= 0.2f; //GROESSE der NORMALEN
		normal += origin;
		nPositions.push_back(origin);
		nPositions.push_back(normal);
	}
	if (maya) {
		for (int i = 0; i < vNormals.size(); i++) {
			glm::vec3 origin = { heVertices[i]->x * scale, heVertices[i]->y * scale, heVertices[i]->z * scale };
			glm::vec3 normal = vNormals[i];
			//normal *= 0.2f; //GROESSE der NORMALEN
			normal += origin;
			nPositions.push_back(origin);
			nPositions.push_back(normal);
		}
	}
	helpNormals = new HNormals(hnPositions, glm::vec3(1.0f, 1.0f, 0.0f));
	normals = new Normals(nPositions, glm::vec3(0.0f, 0.0f, 1.0f));
	glm::vec3 invMiddlepoint = { -(xlength / 2 + bbox[0])*scale, -(ylength / 2 + bbox[2])*scale, -(zlength / 2 + bbox[4])*scale };
	translate(invMiddlepoint);
}

void Model::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
{
	if (!program.compileShaderFromFile(vert.c_str(), GLSLShader::VERTEX))
	{
		throw std::runtime_error(program.log());
	}

	if (!program.compileShaderFromFile(frag.c_str(), GLSLShader::FRAGMENT))
	{
		throw std::runtime_error(program.log());
	}

	if (!program.link())
	{
		throw std::runtime_error(program.log());
	}
}

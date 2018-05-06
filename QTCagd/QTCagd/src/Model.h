#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "GLSLProgram.h"
#include "Object.h"
#include "ObjectLoader.h"
#include "BBox.h"
#include "HNormals.h"
#include "Normals.h"

class Model
{
public:
	// Create a Model from obj file.
	Model(char *path, float size, glm::vec3 color, glm::vec3 position, bool maya);
	~Model();
	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hnorms, const bool norms);
	void translate(glm::vec3 vector);
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis);
	void faceDirection(glm::vec3 direction);
	void reset();
	void runTests();
	void showStatistics();
private:
	void initObject(const char *path, const glm::vec3& color, const float size, bool maya);
	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag);
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object customModel;
	float scale;
	std::vector<HE_vert*> heVertices;
	std::vector<HE_face*> heFaces;
	std::vector<HE_edge*> heEdges;
	std::vector<std::vector<glm::vec3>> vHnormals;
	std::vector<glm::vec3> vNormals;
	BBox *boundingBox;
	HNormals *helpNormals;
	Normals *normals;
};
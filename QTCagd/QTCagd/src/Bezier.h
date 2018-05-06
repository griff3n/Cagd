#pragma once

#include "GLSLProgram.h"
#include "Object.h"
#include "Sphere.h"
#include "BCurve.h"
#include "CasteljauSteps.h"
#include "BBox.h"
#include "Model.h"

class Bezier
{
public:
	// Create a Bezier Curve from obj file.
	Bezier(char *path, float size, glm::vec3 color, int steps);
	~Bezier();
	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hpts, const bool hnorms, const bool norms);
	void translate(glm::vec3 vector);
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis);
	void calculateBezierCurve(const int steps);
	void updateCone(const int steps);
private:
	void initObject(const char *path, const glm::vec3& color, const float size, int steps);
	glm::vec3 deCasteljau(const float t, std::vector<glm::vec3> contrPoints);
	std::vector<glm::vec3> deCasteljauStep(const float t, std::vector<glm::vec3> contrPoints);
	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag);
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object controlCurve;
	std::vector<Sphere*> controlPoints;
	std::vector<glm::vec3> vControlPoints;
	BCurve *bezierCurve;
	CasteljauSteps *helpLines;
	//cg::Cone *cone;
	Model *rocket;
	BBox *boundingBox;
};

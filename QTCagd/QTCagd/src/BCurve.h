#pragma once

#include "GLSLProgram.h"
#include "Object.h"

class BCurve
{
public:
	// Create a vertical line with the given color.
	BCurve(std::vector<glm::vec3> positions, glm::vec3 color);
	void render(const glm::mat4& view, const glm::mat4& projection);
	void translate(glm::vec3 vector);
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis);
private:
	void initObject(const std::vector<glm::vec3> bPositions, const glm::vec3& color);
	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag);
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};
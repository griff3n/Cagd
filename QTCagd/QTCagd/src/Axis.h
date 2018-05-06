#pragma once

#include "GLSLProgram.h"
#include "Object.h"

class Axis
{
public:
	// Create a vertical line with the given color.
	Axis(float size, glm::vec3 color, glm::vec3 position);
	void render(const glm::mat4& view, const glm::mat4& projection);
	void translate(glm::vec3 vector);
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis);
private:
	void initObject(const glm::vec3& color);
	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag);
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "GLSLProgram.h"
#include "Object.h"

class Orbit
{
public:
	// Create a circle with the given color.
	Orbit(float size, glm::vec3 color, glm::vec3 position);
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
	Object circle;
};

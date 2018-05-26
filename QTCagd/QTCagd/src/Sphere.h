#pragma once

//#include <algorithm>

#include <vector>
#include "GLSLProgram.h"
#include "Object.h"


class Sphere
{
public:
	// Create a wired sphere with the given color.
	Sphere(const glm::vec3& color = glm::vec3(0.9f));
	void render(const glm::mat4& view, const glm::mat4& projection);
private:
	void initObject(const glm::vec3& color);
	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag);
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object objSphere;
};
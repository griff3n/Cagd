#pragma once

#include "Sphere.h"
#include "Orbit.h"
#include "Axis.h"
#include "Model.h"

#define FIXEDAXIS true

class Orb
{
public:
	// Create a container for Spheres.
	Orb(glm::vec3 position, float startingRotation, float scaling, glm::vec3 positionOfAxis, glm::vec3 color);
	~Orb();
	void translate(glm::vec3 vector);
	void move(glm::vec3 vector);
	glm::vec3 getPosition();
	glm::vec3 getAxis();
	void onlyRotateCustomModel(float rot_speed, glm::vec3 axis);
	void rotate(float rot_speed);
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis);
	void tilt(float angle);
	void scale(float factor);
	void addChild(Orb* child);
	void addOrbit(float size, const glm::vec3& color);
	void addAxis(float size, const glm::vec3& color);
	void addCustomModel(char *path, float size, const glm::vec3& color);
	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hnorms, const bool norms);
public:
	// Only public for easy access.
	Sphere* sphere;
private:
	std::vector<Orb*> childs;
	std::vector<Orbit*> orbits;
	Axis* axis = nullptr;
	Model* customModel = nullptr;
	Orb* parent = nullptr;
};
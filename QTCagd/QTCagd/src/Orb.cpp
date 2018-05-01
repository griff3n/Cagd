#include "Orb.h"

// Create a container for Spheres.
Orb::Orb(glm::vec3 position, float startingRotation, float scaling, glm::vec3 positionOfAxis, glm::vec3 color)
{
	sphere = new Sphere(color);
	scale(scaling);
	translate(position);
	rotateAroundAxis(startingRotation, glm::vec3(0.0f, 1.0f, 0.0f), positionOfAxis);
}

Orb::~Orb() {
	delete sphere;
	for (Orbit* orbit : orbits) {
		delete orbit;
	}
	if (axis != nullptr) {
		delete axis;
	}
}

void Orb::translate(glm::vec3 vector) {
	sphere->model = sphere->model * glm::translate(vector);
}

void Orb::move(glm::vec3 vector) {
	translate(vector);
	for (Orb* child : childs) {
		child->move(vector);
	}
	for (Orbit* orbit : orbits) {
		orbit->translate(vector);
	}
	if (axis != nullptr) {
		axis->translate(vector);
	}
	if (customModel != nullptr) {
		customModel->translate(vector);
	}
}

glm::vec3 Orb::getPosition() {
	glm::vec4 position = sphere->model * glm::vec4(0, 0, 0, 1);
	glm::vec3 position3 = glm::vec3(position[0], position[1], position[2]);
	return position3;
}

glm::vec3 Orb::getAxis() {
	glm::vec4 position = sphere->model * glm::vec4(0, 0, 0, 1);
	glm::vec3 position3 = glm::vec3(position[0], position[1], position[2]);
	glm::vec4 top = sphere->model * glm::vec4(0, 1, 0, 1);
	glm::vec3 top3 = glm::vec3(top[0], top[1], top[2]);
	glm::vec3 axis = top3 - position3;
	return axis;
}

void Orb::onlyRotateCustomModel(float rot_speed, glm::vec3 axis) {
	if (this->customModel != nullptr) {
		glm::vec3 position = getPosition();
		glm::vec3 invPosition = position * (-1.0f);
		this->customModel->rotateAroundAxis(rot_speed, axis, position);
	}
}

void Orb::rotate(float rot_speed) {

	glm::vec3 position = getPosition();
	glm::vec3 invPosition = position * (-1.0f);
	translate(invPosition);
	glm::vec3 axis = getAxis();
	sphere->model = sphere->model * glm::rotate(rot_speed, axis);
	translate(position);
	for (Orb* child : childs) {
		child->rotateAroundAxis(rot_speed, axis, position);
		if (FIXEDAXIS) {
			glm::vec3 childPos = child->getPosition();
			child->rotateAroundAxis(-rot_speed, axis, childPos);
		}
	}
	if (customModel != nullptr) {
		customModel->rotateAroundAxis(rot_speed, axis, position);
	}
}

void Orb::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	sphere->model = sphere->model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
	for (Orb* child : childs) {
		child->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	}
	for (Orbit* orbit : orbits) {
		orbit->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	}
	if (this->axis != nullptr) {
		this->axis->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	}
	if (this->customModel != nullptr) {
		this->customModel->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	}
}

void Orb::tilt(float angle) {
	glm::vec3 position = getPosition();
	glm::vec3 axis;
	if (FIXEDAXIS) {
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	else if (parent != nullptr) {
		//Calculate axis tangential to orbit
		glm::vec3 parentToChild = position - parent->getPosition();
		glm::vec3 parentAxis = parent->getAxis();
		axis = glm::cross(parentAxis, parentToChild);
	}
	else {
		//Rotate around z-axis
		axis = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	rotateAroundAxis(angle, axis, position);
}

void Orb::scale(float factor) {
	sphere->model = glm::scale(sphere->model, glm::vec3(factor));
}

void Orb::addChild(Orb * child) {
	childs.push_back(child);
	child->parent = this;
}

void Orb::addOrbit(float size, const glm::vec3 & color) {
	glm::vec3 position = getPosition();
	orbits.push_back(new Orbit(size, color, position));
}

void Orb::addAxis(float size, const glm::vec3 & color) {
	if (axis == nullptr) {
		glm::vec3 position = getPosition();
		axis = new Axis(size, color, position);
	}
}

void Orb::addCustomModel(char * path, float size, const glm::vec3 & color) {
	if (customModel == nullptr) {
		glm::vec3 position = getPosition();
		customModel = new Model(path, size, color, position, false);
		customModel->runTests();
		customModel->showStatistics();
	}
}

void Orb::render(const glm::mat4 & view, const glm::mat4 & projection, const bool bbox, const bool hnorms, const bool norms) {
	if (this->customModel != nullptr) {
		customModel->render(view, projection, bbox, hnorms, norms);
	}
	else {
		sphere->render(view, projection);
		if (this->axis != nullptr) {
			axis->render(view, projection);
		}
	}
	for (Orbit* orbit : orbits) {
		orbit->render(view, projection);
	}
}

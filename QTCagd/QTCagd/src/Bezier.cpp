#include "Bezier.h"

// Create a Bezier Curve from obj file.
Bezier::Bezier(char * path, float size, glm::vec3 color, int steps)
{
	Bezier::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");
	initObject(path, color, size, steps);
}

Bezier::~Bezier() {
	delete boundingBox;
	//FEHLT ETWAS
}

void Bezier::render(const glm::mat4 & view, const glm::mat4 & projection, const bool bbox, const bool hpts, const bool hnorms, const bool norms)
{
	for (Sphere *cpt : controlPoints) {
		cpt->render(view, projection);
	}
	if (bbox) boundingBox->render(view, projection);
	bezierCurve->render(view, projection);
	if (helpLines && hpts) helpLines->render(view, projection);
	//cone->render(view, projection);
	rocket->render(view, projection, bbox, hnorms, norms);
	// Create mvp.
	//glm::mat4 mvp = projection * view * model;

	// Bind the shader program and set uniform(s).
	//programSimple.use();
	//programSimple.setUniform("mvp", mvp);

	// Bind vertex array object.
	//glBindVertexArray(controlCurve.vao);
	//glDrawElements(GL_LINES, controlCurve.indexCount, GL_UNSIGNED_INT, 0);
	//glBindVertexArray(0);
}

void Bezier::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
	boundingBox->translate(vector);
	for (Sphere *cpt : controlPoints) {
		cpt->model = cpt->model * glm::translate(vector);
	}
}

void Bezier::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
	boundingBox->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	for (Sphere *cpt : controlPoints) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		cpt->model = cpt->model * glm::rotate(rot_speed, axis);
		translate(positionOfAxis);
	}
}

void Bezier::calculateBezierCurve(const int steps)
{
	if (bezierCurve) delete bezierCurve;
	// Build the model.
	std::vector<glm::vec3> bPositions;
	bPositions.push_back(vControlPoints[0]);
	for (int i = 1; i < steps; i++) {
		float t = (float)i / steps;
		glm::vec3 newPoint = deCasteljau(t, vControlPoints);
		bPositions.push_back(newPoint);
	}
	bPositions.push_back(vControlPoints[vControlPoints.size() - 1]);

	std::vector<glm::vec3> positions;
	for (glm::vec3 pos : bPositions) {
		glm::vec4 pos4(pos, 1.0f);
		glm::vec4 actualPos4 = model * pos4;
		glm::vec3 actualPos = { actualPos4.x, actualPos4.y, actualPos4.z };
		positions.push_back(actualPos);
	}

	bezierCurve = new BCurve(positions, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Bezier::updateCone(const int steps) {

	static int lastSteps = steps;
	static int i = 1;
	if (lastSteps != steps) {
		lastSteps = steps;
		i = 1;
	}
	float t = (float)i / steps;
	//
	std::vector<std::vector<glm::vec3>> deCasteljauPoints;
	while (1) {
		std::vector<glm::vec3> iterationStep;
		if (deCasteljauPoints.size() == 0) {
			iterationStep = deCasteljauStep(t, vControlPoints);
		}
		else {
			int size = deCasteljauPoints.size();
			iterationStep = deCasteljauStep(t, deCasteljauPoints[size - 1]);
		}
		deCasteljauPoints.push_back(iterationStep);
		if (iterationStep.size() == 1) break;
	}
	//Build Help Lines
	std::vector<glm::vec3> hpositions;
	for (int j = 0; j < deCasteljauPoints.size() - 1; j++) {
		for (int k = 0; k < deCasteljauPoints[j].size() - 1; k++) {
			glm::vec3 pos1 = deCasteljauPoints[j][k];
			glm::vec3 pos2 = deCasteljauPoints[j][k + 1];

			glm::vec4 pos4 = glm::vec4(pos1, 1.0f);
			glm::vec4 actualPos4 = model * pos4;
			glm::vec3 actualPos = { actualPos4.x, actualPos4.y, actualPos4.z };
			hpositions.push_back(actualPos);

			pos4 = glm::vec4(pos2, 1.0f);
			actualPos4 = model * pos4;
			actualPos = { actualPos4.x, actualPos4.y, actualPos4.z };
			hpositions.push_back(actualPos);
		}
	}
	if (helpLines != nullptr) delete helpLines;
	helpLines = new CasteljauSteps(hpositions, glm::vec3(0.0f, 1.0f, 1.0f));

	//Move the Cone and set orientation
	//cone->reset();
	rocket->reset();
	glm::vec3 directionOfDifferentiation = deCasteljauPoints[deCasteljauPoints.size() - 2][1] - deCasteljauPoints[deCasteljauPoints.size() - 2][0];
	//cone->faceDirection(directionOfDifferentiation);
	rocket->faceDirection(directionOfDifferentiation);
	glm::vec3 positionOfCone = deCasteljauPoints[deCasteljauPoints.size() - 1][0];
	glm::vec4 pos4 = glm::vec4(positionOfCone, 1.0f);
	glm::vec4 actualPos4 = model * pos4;
	glm::vec3 actualPos = { actualPos4.x, actualPos4.y, actualPos4.z };
	//cone->translate(actualPos);
	rocket->translate(actualPos);
	//
	i += 1;
	if (i == steps) i = 1;
}

void Bezier::initObject(const char * path, const glm::vec3 & color, const float size, int steps)
{
	// Build the model.
	std::vector<glm::vec3> positions;
	std::vector<GLuint> indices;
	std::vector<GLuint> bIndices;
	std::vector<float> bbox;
	bbox.resize(6);
	if (!loadBezier(path, positions, bIndices, bbox)) {
		positions.push_back(glm::vec3({ 0.0f, 1.0f, 0.0f }));
		positions.push_back(glm::vec3({ 0.0f, -1.0f, 0.0f }));
		positions.push_back(glm::vec3({ 1.0f, 0.0f, 0.0f }));
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
	}
	boundingBox = new BBox(bbox, glm::vec3(1.0f, 1.0f, 1.0f));

	for (int i = 0; i < bIndices.size() - 1; i++) {
		indices.push_back(bIndices[i]);
		indices.push_back(bIndices[i + 1]);
	}

	std::vector<glm::vec3> colors;

	for (std::size_t i = 0; i < positions.size(); i++)
		colors.push_back(color);
	//{
	//	if (i % 2 == 0)colors.push_back(color);
	//	else colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	//}

	GLuint programId = programSimple.getHandle();
	GLuint pos;

	// Vertex array object.
	glGenVertexArrays(1, &controlCurve.vao);
	glBindVertexArray(controlCurve.vao);

	// Position buffer.
	glGenBuffers(1, &controlCurve.positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, controlCurve.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Color buffer.
	glGenBuffers(1, &controlCurve.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, controlCurve.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index buffer.
	controlCurve.indexCount = (GLuint)indices.size();

	glGenBuffers(1, &controlCurve.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, controlCurve.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, controlCurve.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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
	model = glm::scale(model, glm::vec3(scale));
	boundingBox->model = glm::scale(boundingBox->model, glm::vec3(scale));

	for (int i = 0; i < positions.size(); i++) {
		Sphere *controlPoint = new Sphere(glm::vec3(1.0f, 0.0f, 1.0f));
		controlPoint->model = glm::scale(controlPoint->model, glm::vec3(0.1f));
		controlPoint->model = controlPoint->model * glm::translate(positions[i] * scale);
		controlPoints.push_back(controlPoint);
	}

	glm::vec3 invMiddlepoint = { -(xlength / 2 + bbox[0])*scale, -(ylength / 2 + bbox[2])*scale, -(zlength / 2 + bbox[4])*scale };
	translate(invMiddlepoint);

	for (int i = 0; i < bIndices.size(); i++) {
		vControlPoints.push_back(positions[bIndices[i]]);
	}
	calculateBezierCurve(steps);
	//cone = new cg::Cone(0.1f, glm::vec3(1.0f, 0.5f, 0.0f));
	rocket = new Model("Rocket_soften.obj", 1.0f, glm::vec3(1.0f, 0.5f, 0.0f), glm::vec3(0.0f), true);
}

glm::vec3 Bezier::deCasteljau(const float t, std::vector<glm::vec3> contrPoints) {
	std::vector<glm::vec3> nextPoints;
	for (int i = 0; i < contrPoints.size() - 1; i++) {
		glm::vec3 newPoint = contrPoints[i] * (1 - t) + contrPoints[i + 1] * t;
		nextPoints.push_back(newPoint);
	}
	if (nextPoints.size() == 1) {
		return nextPoints[0];
	}
	else {
		return deCasteljau(t, nextPoints);
	}
}

std::vector<glm::vec3> Bezier::deCasteljauStep(const float t, std::vector<glm::vec3> contrPoints) {
	std::vector<glm::vec3> nextPoints;
	for (int i = 0; i < contrPoints.size() - 1; i++) {
		glm::vec3 newPoint = contrPoints[i] * (1 - t) + contrPoints[i + 1] * t;
		nextPoints.push_back(newPoint);
	}
	return nextPoints;
}

void Bezier::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
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

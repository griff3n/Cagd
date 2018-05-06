#include "Cone.h"

// Create a cone with the given color.
Cone::Cone(float size, glm::vec3 color)
{
	Cone::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");
	initObject(color);
	model = glm::scale(model, glm::vec3(size));
}

void Cone::render(const glm::mat4 & view, const glm::mat4 & projection)
{
	// Create mvp.
	glm::mat4 mvp = projection * view * model;

	// Bind the shader program and set uniform(s).
	programSimple.use();
	programSimple.setUniform("mvp", mvp);

	// Bind vertex array object.
	glBindVertexArray(cone.vao);
	glDrawElements(GL_TRIANGLES, cone.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Cone::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
}

void Cone::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
}

void Cone::faceDirection(glm::vec3 direction) {
	if (direction.x == 0.0f && direction.z == 0.0f) {
		if (direction.y < 0) rotateAroundAxis(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
		return;
	}
	float angleY = glm::acos(glm::dot(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
	rotateAroundAxis(angleY, axis, glm::vec3(0.0f));
}

void Cone::initObject(const glm::vec3 & color)
{
	// Build the cone.
	std::vector<glm::vec3> positions = { { 0.0, 2.0, 0.0 },{ 1.0, 0.0, 0.0 },{ 0.707107, 0.0, -0.707107 },{ 0.0, 0.0, -1.0 },{ -0.707107, 0.0, -0.707107 },{ -1.0, 0.0, 0.0 },{ -0.707107, 0.0, 0.707107 },{ 0.0, 0.0, 1.0 },{ 0.707107, 0.0, 0.707107 } };
	std::vector<GLuint> indices = { 0,1,2, 0,2,3, 0,3,4, 0,4,5, 0,5,6, 0,6,7, 0,7,8, 0,8,1, 1,8,7, 1,7,6, 1,6,5, 1,5,4, 1,4,3, 1,3,2 };

	std::vector<glm::vec3> colors;
	for (std::size_t i = 0; i < positions.size(); i++)
		colors.push_back(color);

	GLuint programId = programSimple.getHandle();
	GLuint pos;

	// Vertex array object.
	glGenVertexArrays(1, &cone.vao);
	glBindVertexArray(cone.vao);

	// Position buffer.
	glGenBuffers(1, &cone.positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cone.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Color buffer.
	glGenBuffers(1, &cone.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cone.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index buffer.
	cone.indexCount = (GLuint)indices.size();

	glGenBuffers(1, &cone.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cone.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Cone::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
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

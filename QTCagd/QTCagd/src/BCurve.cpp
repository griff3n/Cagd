#include "BCurve.h"

// Create a vertical line with the given color.
BCurve::BCurve(std::vector<glm::vec3> positions, glm::vec3 color)
{
	BCurve::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");
	initObject(positions, color);
}

void BCurve::render(const glm::mat4 & view, const glm::mat4 & projection)
{
	// Create mvp.
	glm::mat4 mvp = projection * view * model;

	// Bind the shader program and set uniform(s).
	programSimple.use();
	programSimple.setUniform("mvp", mvp);

	// Bind vertex array object.
	glBindVertexArray(line.vao);
	glDrawElements(GL_LINES, line.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void BCurve::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
}

void BCurve::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
}

void BCurve::initObject(const std::vector<glm::vec3> bPositions, const glm::vec3 & color)
{
	// Build the bezier curve.
	std::vector<glm::vec3> positions = bPositions;
	std::vector<GLuint> indices;

	for (int i = 0; i < positions.size() - 1; i++) {
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	std::vector<glm::vec3> colors;

	for (std::size_t i = 0; i < positions.size(); i++)
		colors.push_back(color);

	GLuint programId = programSimple.getHandle();
	GLuint pos;

	// Vertex array object.
	glGenVertexArrays(1, &line.vao);
	glBindVertexArray(line.vao);

	// Position buffer.
	glGenBuffers(1, &line.positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, line.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Color buffer.
	glGenBuffers(1, &line.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, line.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index buffer.
	line.indexCount = (GLuint)indices.size();

	glGenBuffers(1, &line.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, line.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, line.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void BCurve::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
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

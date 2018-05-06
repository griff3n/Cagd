#include "BBox.h"

// Create a vertical line with the given color.
BBox::BBox(std::vector<float> bbox, glm::vec3 color)
{
	BBox::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

	initObject(bbox, color);
}

void BBox::render(const glm::mat4 & view, const glm::mat4 & projection)
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

void BBox::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
}

void BBox::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
}

void BBox::initObject(const std::vector<float> bbox, const glm::vec3 & color)
{
	// Build the axis.
	std::vector<glm::vec3> positions = { { bbox[0], bbox[2], bbox[5] },{ bbox[1], bbox[2], bbox[5] },{ bbox[1], bbox[3], bbox[5] },{ bbox[0], bbox[3], bbox[5] },{ bbox[0], bbox[3], bbox[4] },{ bbox[1], bbox[3], bbox[4] },{ bbox[1], bbox[2], bbox[4] },{ bbox[0], bbox[2], bbox[4] } };
	std::vector<GLuint> indices = { 0, 1, 1, 2, 2, 3, 3, 0, 2, 5, 5, 4, 4, 3, 5, 6, 6, 7, 7, 4, 6, 1, 0, 7 }; //{ 1, 2, 3, 4 }, { 4, 3, 6, 5 }, { 5, 6, 7, 8 }, { 8, 7, 2, 1 }, { 2, 7, 6, 3 }, { 8, 1, 4, 5 }
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

void BBox::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
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

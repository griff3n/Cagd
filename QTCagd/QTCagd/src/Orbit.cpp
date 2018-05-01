#include "Orbit.h"

// Create a circle with the given color.
Orbit::Orbit(float size, glm::vec3 color, glm::vec3 position)
{
	Orbit::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");
	initObject(color);
	model = glm::scale(model, glm::vec3(size));
	translate(position);
}

void Orbit::render(const glm::mat4 & view, const glm::mat4 & projection)
{
	// Create mvp.
	glm::mat4 mvp = projection * view * model;

	// Bind the shader program and set uniform(s).
	programSimple.use();
	programSimple.setUniform("mvp", mvp);

	// Bind vertex array object.
	glBindVertexArray(circle.vao);
	glDrawElements(GL_LINES, circle.indexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Orbit::translate(glm::vec3 vector) {
	model = model * glm::translate(vector);
}

void Orbit::rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
	glm::vec3 invPosition = positionOfAxis * (-1.0f);
	translate(invPosition);
	model = model * glm::rotate(rot_speed, axis);
	translate(positionOfAxis);
}

void Orbit::initObject(const glm::vec3 & color)
{
	// Build the circle.
	std::vector<glm::vec3> colors;
	std::vector<glm::vec3> positions;
	int accuracy = 360;
	for (int i = 0; i < accuracy; i++) {
		float grad = ((float)i / accuracy) * 2.0f * glm::pi<float>();
		positions.push_back({ cos(grad), 0, sin(grad) });
	}
	std::vector<GLuint> indices;
	for (int i = 0; i < accuracy; i++) {
		indices.push_back(i);
		if (i != accuracy - 1) {
			indices.push_back(i + 1);
		}
		else { indices.push_back(0); }
	}

	for (std::size_t i = 0; i < positions.size(); i++)
		colors.push_back(color);

	GLuint programId = programSimple.getHandle();
	GLuint pos;

	// Vertex array object.
	glGenVertexArrays(1, &circle.vao);
	glBindVertexArray(circle.vao);

	// Position buffer.
	glGenBuffers(1, &circle.positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, circle.positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "position");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Color buffer.
	glGenBuffers(1, &circle.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, circle.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	pos = glGetAttribLocation(programId, "color");
	glEnableVertexAttribArray(pos);
	glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index buffer.
	circle.indexCount = (GLuint)indices.size();

	glGenBuffers(1, &circle.indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, circle.indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, circle.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void Orbit::initShader(GLSLProgram & program, const std::string & vert, const std::string & frag)
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

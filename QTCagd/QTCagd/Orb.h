#pragma once

#include "Sphere.h"
#include "ObjectLoader.h"

#define FIXEDAXIS true

class CasteljauSteps
{
public:
	// Create a vertical line with the given color.
	CasteljauSteps(std::vector<glm::vec3> positions, glm::vec3 color)
	{
		CasteljauSteps::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(positions, color);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const std::vector<glm::vec3> bPositions, const glm::vec3& color)
	{
		// Build the bezier curve.
		std::vector<glm::vec3> positions = bPositions;
		std::vector<GLuint> indices;

		for (int i = 0; i < positions.size(); i++) {
			indices.push_back(i);
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class Cone
{
public:
	// Create a cone with the given color.
	Cone(float size, glm::vec3 color)
	{
		Cone::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(color);
		model = glm::scale(model, glm::vec3(size));
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
	void faceDirection(glm::vec3 direction) {
		if (direction.x == 0.0f && direction.z == 0.0f) {
			if (direction.y < 0) rotateAroundAxis(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
			return;
		}
		float angleY = glm::acos(glm::dot(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
		rotateAroundAxis(angleY, axis, glm::vec3(0.0f));
	}
private:
	void initObject(const glm::vec3& color)
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object cone;
};

class BCurve
{
public:
	// Create a vertical line with the given color.
	BCurve(std::vector<glm::vec3> positions, glm::vec3 color)
	{
		BCurve::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(positions, color);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const std::vector<glm::vec3> bPositions, const glm::vec3& color)
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class HNormals
{
public:
	// Create a vertical line with the given color.
	HNormals(std::vector<glm::vec3> positions, glm::vec3 color)
	{
		HNormals::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(positions, color);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const std::vector<glm::vec3> hnPositions, const glm::vec3& color)
	{
		// Build the axis.
		std::vector<glm::vec3> positions = hnPositions;
		std::vector<GLuint> indices;
		for (int counter = 0; counter < positions.size(); counter++) {
			indices.push_back(counter);
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class Normals
{
public:
	// Create a vertical line with the given color.
	Normals(std::vector<glm::vec3> positions, glm::vec3 color)
	{
		Normals::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(positions, color);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const std::vector<glm::vec3> nPositions, const glm::vec3& color)
	{
		// Build the axis.
		std::vector<glm::vec3> positions = nPositions;
		std::vector<GLuint> indices;
		for (int counter = 0; counter < positions.size(); counter++) {
			indices.push_back(counter);
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class BBox
{
public:
	// Create a vertical line with the given color.
	BBox(std::vector<float> bbox, glm::vec3 color)
	{
		BBox::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(bbox, color);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const std::vector<float> bbox, const glm::vec3& color)
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class Model
{
public:
	// Create a Model from obj file.
	Model(char *path, float size, glm::vec3 color, glm::vec3 position, bool maya)
	{
		Model::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(path, color, size, maya);
		translate(position);
	}

	~Model() {
		delete boundingBox;
		delete helpNormals;
		delete normals;
		for (HE_vert* vertex : heVertices) {
			delete vertex;
		}
		for (HE_edge* edge : heEdges) {
			delete edge;
		}
		for (HE_face* face : heFaces) {
			delete face;
		}
	}

	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hnorms, const bool norms)
	{
		if (bbox) boundingBox->render(view, projection);
		if (hnorms) helpNormals->render(view, projection);
		if (norms) normals->render(view, projection);
		// Create mvp.
		glm::mat4 mvp = projection * view * model;

		// Bind the shader program and set uniform(s).
		programSimple.use();
		programSimple.setUniform("mvp", mvp);

		// Bind vertex array object.
		glBindVertexArray(customModel.vao);
		glDrawElements(GL_TRIANGLES, customModel.indexCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
		boundingBox->translate(vector);
		helpNormals->translate(vector);
		normals->translate(vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
		boundingBox->rotateAroundAxis(rot_speed, axis, positionOfAxis);
		helpNormals->rotateAroundAxis(rot_speed, axis, positionOfAxis);
		normals->rotateAroundAxis(rot_speed, axis, positionOfAxis);
	}
	void faceDirection(glm::vec3 direction) {
		//model *= glm::orientation(direction, glm::vec3(0.0f, 0.0f, 1.0f));

		if (direction.x == 0.0f && direction.z == 0.0f) {
			if (direction.y < 0) rotateAroundAxis(glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f));
			return;
		}
		float angleY = glm::acos(glm::dot(glm::normalize(direction), glm::vec3(0.0f, 1.0f, 0.0f)));
		glm::vec3 axis = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), direction);
		rotateAroundAxis(angleY, axis, glm::vec3(0.0f));

	}
	void reset() {
		glm::mat4 unitMatrix;
		model = glm::scale(unitMatrix, glm::vec3(scale));
		boundingBox->model = glm::scale(unitMatrix, glm::vec3(scale));
		helpNormals->model = glm::scale(unitMatrix, glm::vec3(1.0));
		normals->model = glm::scale(unitMatrix, glm::vec3(1.0));
	}
	void runTests() {
		std::cout << "Tests =============================================" << std::endl;
		//HALFEDGE TEST
		bool error = false;
		for (int i = 0; i < heEdges.size(); i++) {
			if (heEdges[i]->pair == nullptr) {
				error = true;
				std::cout << "ERROR - Halfedges - Eine Halfedge hat kein pair!" << std::endl;
				break;
			}
			else if (heEdges[i]->pair->pair == nullptr) {
				error = true;
				std::cout << "ERROR - Halfedges - Eine Halfedge hat kein pair!" << std::endl;
				break;
			}
			else if (heEdges[i]->pair->pair != heEdges[i]) {
				error = true;
				std::cout << "ERROR - Halfedges - Ein Pair ist falsch gesetzt!" << std::endl;
				break;
			}
		}
		if (!error) std::cout << "OK - Halfedges - Alle pairs wurden konsistent gesetzt!" << std::endl;
		//FACES TEST
		error = false;
		for (int i = 0; i < heFaces.size(); i++) {
			if (heFaces[i]->edge == nullptr) {
				error = true;
				std::cout << "ERROR - Faces - Ein Face hat keine edge!" << std::endl;
				break;
			}
			else {
				HE_edge *tempEdge = heFaces[i]->edge;
				for (int j = 0; j < heFaces[i]->valence; j++) {
					if (tempEdge->next == nullptr) {
						error = true;
						std::cout << "ERROR - Faces - Eine Halfedge des Faces hat kein next!" << std::endl;
						break;
					}
					tempEdge = tempEdge->next;
				}
				if (error) break;
				if (tempEdge != heFaces[i]->edge) {
					error = true;
					std::cout << "ERROR - Faces - Die Halfedges des Faces sind nicht richtig verknüpft!" << std::endl;
					break;
				}
				for (int j = 0; j < heFaces[i]->valence; j++) {
					if (tempEdge->face != heFaces[i]) {
						error = true;
						std::cout << "ERROR - Faces - Eine Halfedge des Faces ist nicht mit dem Face verbunden!" << std::endl;
						break;
					}
					if (tempEdge->pair == nullptr) {
						error = true;
						std::cout << "ERROR - Faces - Eine Halfedge des Faces hat kein pair!" << std::endl;
						break;
					}
					else if (tempEdge->pair->vert == nullptr) {
						error = true;
						std::cout << "ERROR - Faces - Eine Halfedge hat keinen vert!" << std::endl;
						break;
					}
					else if (tempEdge->next->vert == nullptr) {
						error = true;
						std::cout << "ERROR - Faces - Eine Halfedge des Faces hat keinen vert!" << std::endl;
						break;
					}
					else if (tempEdge->pair->vert != tempEdge->next->vert) {
						error = true;
						std::cout << "ERROR - Faces - Ein Vertex des Faces ist nicht konsistent mit seinen Halfedges verbunden!" << std::endl;
						break;
					}
					tempEdge = tempEdge->next;
				}
				if (error) break;
			}
		}
		if (!error) std::cout << "OK - Faces - Alle Vertices sind konsistent mit ihren Halfedges verbunden und alle Halfedges sind konsistent mit ihrem Face verbunden!" << std::endl;
		//TEST VERTICES
		error = false;
		for (int i = 0; i < heVertices.size(); i++) {
			if (heVertices[i]->edge == nullptr) {
				error = true;
				std::cout << "ERROR - Vertices - Ein Vertex hat kein edge!" << std::endl;
				break;
			}
			else {
				HE_edge *tempEdge = heVertices[i]->edge;
				for (int j = 0; j < heVertices[i]->valence; j++) {
					if (tempEdge->pair == nullptr) {
						error = true;
						std::cout << "ERROR - Vertices - Eine angrenzende Halfedge hat kein pair!" << std::endl;
						break;
					}
					tempEdge = tempEdge->pair;
					if (tempEdge->next == nullptr) {
						error = true;
						std::cout << "ERROR - Vertices - Eine angrenzende Halfedge hat kein next!" << std::endl;
						break;
					}
					tempEdge = tempEdge->next;
				}
				if (error) break;
				if (tempEdge != heVertices[i]->edge) {
					error = true;
					std::cout << "ERROR - Vertices - Die angrenzenden Halfedges des Vertices sind nicht richtig verknüpft!" << std::endl;
					break;
				}
			}
		}
		if (!error) std::cout << "OK - Vertices - Alle angrenzenden Halfedges sind erreichbar!" << std::endl;
		std::cout << "===================================================" << std::endl;
		std::cout << std::endl;
	}
	void showStatistics() {
		std::cout << "Statistik =========================================" << std::endl;
		std::cout << "Vertices: " << heVertices.size() << std::endl;
		std::vector<int> vertexValences;
		for (int i = 0; i < heVertices.size(); i++) {
			if (heVertices[i]->valence + 1 > vertexValences.size()) vertexValences.resize(heVertices[i]->valence + 1);
			vertexValences[heVertices[i]->valence] ++;
		}
		for (int i = 0; i < vertexValences.size(); i++) {
			if (vertexValences[i] > 0) {
				std::cout << "Vertex-Valenz " << i << ": " << vertexValences[i] << std::endl;
			}
		}
		std::cout << "Faces: " << heFaces.size() << std::endl;
		std::vector<int> faceValences;
		for (int i = 0; i < heFaces.size(); i++) {
			if (heFaces[i]->valence + 1 > faceValences.size()) faceValences.resize(heFaces[i]->valence + 1);
			faceValences[heFaces[i]->valence] ++;
		}
		for (int i = 0; i < faceValences.size(); i++) {
			if (faceValences[i] > 0) {
				std::cout << "Face-Valenz " << i << ": " << faceValences[i] << std::endl;
			}
		}
		std::cout << "Halfedges: " << heEdges.size() << std::endl;
		std::cout << "===================================================" << std::endl;
		std::cout << std::endl;
	}
private:
	void initObject(const char *path, const glm::vec3& color, const float size, bool maya)
	{
		// Build the model.
		std::vector<glm::vec3> positions;
		std::vector<GLuint> indices;
		std::vector<float> bbox;
		bbox.resize(6);
		if (maya) {
			if (!loadMayaOBJ(path, positions, indices, heVertices, heFaces, heEdges, bbox)) {
				positions.push_back(glm::vec3({ 0.0f, 1.0f, 0.0f }));
				positions.push_back(glm::vec3({ 0.0f, -1.0f, 0.0f }));
				positions.push_back(glm::vec3({ 1.0f, 0.0f, 0.0f }));
				indices.push_back(0);
				indices.push_back(1);
				indices.push_back(2);
			}
		}
		else {
			if (!loadOBJ(path, positions, indices, heVertices, heFaces, heEdges, bbox)) {
				positions.push_back(glm::vec3({ 0.0f, 1.0f, 0.0f }));
				positions.push_back(glm::vec3({ 0.0f, -1.0f, 0.0f }));
				positions.push_back(glm::vec3({ 1.0f, 0.0f, 0.0f }));
				indices.push_back(0);
				indices.push_back(1);
				indices.push_back(2);
			}
		}

		boundingBox = new BBox(bbox, glm::vec3(1.0f, 1.0f, 1.0f));
		if (maya) {
			// Build the Normals.
			vNormals.resize(positions.size());
			for (int i = 0; i < heVertices.size(); i++) {
				vNormals[i] = *heVertices[i]->normal;
			}
		}
		else {
			// Build the helpNormals and Normals.
			vHnormals.resize(positions.size());
			vNormals.resize(positions.size());
			for (int i = 0; i < heVertices.size(); i++) {
				glm::vec3 origin = { heVertices[i]->x, heVertices[i]->y, heVertices[i]->z };
				HE_edge *tempEdge = heVertices[i]->edge;
				std::vector<glm::vec3> hNormals;
				for (int j = 0; j < heVertices[i]->valence; j++) {
					glm::vec3 dir1 = { tempEdge->pair->vert->x, tempEdge->pair->vert->y, tempEdge->pair->vert->z };
					dir1 = dir1 - origin;
					glm::vec3 dir2 = { tempEdge->pair->next->pair->vert->x, tempEdge->pair->next->pair->vert->y, tempEdge->pair->next->pair->vert->z };
					dir2 = dir2 - origin;
					glm::vec3 helpNormal = glm::cross(dir2, dir1);
					hNormals.push_back(helpNormal);
					helpNormal = glm::normalize(helpNormal);
					vHnormals[i].push_back(helpNormal);
					tempEdge = tempEdge->pair->next;
				}
				glm::vec3 normal = { 0.0f, 0.0f, 0.0f };
				for (int j = 0; j < hNormals.size(); j++) {
					normal += hNormals[j];
				}
				normal = glm::normalize(normal);
				vNormals[i] = normal;
			}
		}

		std::vector<glm::vec3> colors;

		for (std::size_t i = 0; i < positions.size(); i++)
			//colors.push_back(color);
		{
			if (i % 2 == 0)colors.push_back(color);
			else colors.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		GLuint programId = programSimple.getHandle();
		GLuint pos;

		// Vertex array object.
		glGenVertexArrays(1, &customModel.vao);
		glBindVertexArray(customModel.vao);

		// Position buffer.
		glGenBuffers(1, &customModel.positionBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, customModel.positionBuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

		pos = glGetAttribLocation(programId, "position");
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Color buffer.
		glGenBuffers(1, &customModel.colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, customModel.colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

		pos = glGetAttribLocation(programId, "color");
		glEnableVertexAttribArray(pos);
		glVertexAttribPointer(pos, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Index buffer.
		customModel.indexCount = (GLuint)indices.size();

		glGenBuffers(1, &customModel.indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, customModel.indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, customModel.indexCount * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

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
		this->scale = scale;
		model = glm::scale(model, glm::vec3(scale));
		boundingBox->model = glm::scale(boundingBox->model, glm::vec3(scale));
		std::vector<glm::vec3> hnPositions;
		std::vector<glm::vec3> nPositions;
		for (int i = 0; i < vHnormals.size(); i++) {
			glm::vec3 origin = { heVertices[i]->x * scale, heVertices[i]->y * scale, heVertices[i]->z * scale };
			for (int j = 0; j < vHnormals[i].size(); j++) {
				glm::vec3 helpNormal = vHnormals[i][j];
				//helpNormal *= 0.2f; //GROESSE der NORMALEN
				helpNormal += origin;
				hnPositions.push_back(origin);
				hnPositions.push_back(helpNormal);
				//std::cout << "origin " << origin.x << " " << origin.y << " " << origin.z << ", helpNormal " << helpNormal.x << " " << helpNormal.y << " " << helpNormal.z << std::endl;
			}
			glm::vec3 normal = vNormals[i];
			//normal *= 0.2f; //GROESSE der NORMALEN
			normal += origin;
			nPositions.push_back(origin);
			nPositions.push_back(normal);
		}
		if (maya) {
			for (int i = 0; i < vNormals.size(); i++) {
				glm::vec3 origin = { heVertices[i]->x * scale, heVertices[i]->y * scale, heVertices[i]->z * scale };
				glm::vec3 normal = vNormals[i];
				//normal *= 0.2f; //GROESSE der NORMALEN
				normal += origin;
				nPositions.push_back(origin);
				nPositions.push_back(normal);
			}
		}
		helpNormals = new HNormals(hnPositions, glm::vec3(1.0f, 1.0f, 0.0f));
		normals = new Normals(nPositions, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::vec3 invMiddlepoint = { -(xlength / 2 + bbox[0])*scale, -(ylength / 2 + bbox[2])*scale, -(zlength / 2 + bbox[4])*scale };
		translate(invMiddlepoint);
	}

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object customModel;
	float scale;
	std::vector<HE_vert*> heVertices;
	std::vector<HE_face*> heFaces;
	std::vector<HE_edge*> heEdges;
	std::vector<std::vector<glm::vec3>> vHnormals;
	std::vector<glm::vec3> vNormals;
	BBox *boundingBox;
	HNormals *helpNormals;
	Normals *normals;
};

class Bezier
{
public:
	// Create a Bezier Curve from obj file.
	Bezier(char *path, float size, glm::vec3 color, int steps)
	{
		Bezier::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(path, color, size, steps);
	}

	~Bezier() {
		delete boundingBox;
		//FEHLT ETWAS
	}

	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hpts, const bool hnorms, const bool norms)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
		boundingBox->translate(vector);
		for (Sphere *cpt : controlPoints) {
			cpt->model = glm::myTranslate(cpt->model, vector);
		}
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
		boundingBox->rotateAroundAxis(rot_speed, axis, positionOfAxis);
		for (Sphere *cpt : controlPoints) {
			glm::vec3 invPosition = positionOfAxis * (-1.0f);
			translate(invPosition);
			cpt->model = glm::myRotate(cpt->model, rot_speed, axis);
			translate(positionOfAxis);
		}
	}

	void calculateBezierCurve(const int steps)
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

	void updateCone(const int steps) {

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

private:
	void initObject(const char *path, const glm::vec3& color, const float size, int steps)
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
			controlPoint->model = glm::myTranslate(controlPoint->model, positions[i] * scale);
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

	glm::vec3 deCasteljau(const float t, std::vector<glm::vec3> contrPoints) {
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

	std::vector<glm::vec3> deCasteljauStep(const float t, std::vector<glm::vec3> contrPoints) {
		std::vector<glm::vec3> nextPoints;
		for (int i = 0; i < contrPoints.size() - 1; i++) {
			glm::vec3 newPoint = contrPoints[i] * (1 - t) + contrPoints[i + 1] * t;
			nextPoints.push_back(newPoint);
		}
		return nextPoints;
	}

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object controlCurve;
	std::vector<Sphere*> controlPoints;
	std::vector<glm::vec3> vControlPoints;
	BCurve *bezierCurve;
	CasteljauSteps *helpLines;
	//cg::Cone *cone;
	Model *rocket;
	BBox *boundingBox;
};

class Axis
{
public:
	// Create a vertical line with the given color.
	Axis(float size, glm::vec3 color, glm::vec3 position)
	{
		Axis::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(color);
		model = glm::scale(model, glm::vec3(size));
		translate(position);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const glm::vec3& color)
	{
		// Build the axis.
		std::vector<glm::vec3> positions = { { 0.0f, 1.0f, 0.0f },{ 0.0f, -1.0f, 0.0f } };
		std::vector<GLuint> indices = { 0,1 };
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object line;
};

class Orbit
{
public:
	// Create a circle with the given color.
	Orbit(float size, glm::vec3 color, glm::vec3 position)
	{
		Orbit::initShader(programSimple, "shader/simple.vert", "shader/simple.frag");

		initObject(color);
		model = glm::scale(model, glm::vec3(size));
		translate(position);
	}

	void render(const glm::mat4& view, const glm::mat4& projection)
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
	void translate(glm::vec3 vector) {
		model = glm::myTranslate(model, vector);
	}
	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		model = glm::myRotate(model, rot_speed, axis);
		translate(positionOfAxis);
	}
private:
	void initObject(const glm::vec3& color)
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

	static void initShader(GLSLProgram& program, const std::string& vert, const std::string& frag)
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
public:
	// Only public for easy access.
	glm::mat4 model;
private:
	GLSLProgram programSimple;
	Object circle;
};

class Orb
{
public:
	// Create a container for Spheres.
	Orb(glm::vec3 position, float startingRotation, float scaling, glm::vec3 positionOfAxis, glm::vec3 color)
	{
		sphere = new Sphere(color);

		scale(scaling);
		translate(position);
		rotateAroundAxis(startingRotation, glm::vec3(0.0f, 1.0f, 0.0f), positionOfAxis);
	}

	~Orb() {
		delete sphere;
		for (Orbit* orbit : orbits) {
			delete orbit;
		}
		if (axis != nullptr) {
			delete axis;
		}
	}

	void translate(glm::vec3 vector) {
		sphere->model = glm::myTranslate(sphere->model, vector);
	}

	void move(glm::vec3 vector) {
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

	glm::vec3 getPosition() {
		glm::vec4 position = sphere->model * glm::vec4(0, 0, 0, 1);
		glm::vec3 position3 = glm::vec3(position[0], position[1], position[2]);
		return position3;
	}

	glm::vec3 getAxis() {
		glm::vec4 position = sphere->model * glm::vec4(0, 0, 0, 1);
		glm::vec3 position3 = glm::vec3(position[0], position[1], position[2]);
		glm::vec4 top = sphere->model * glm::vec4(0, 1, 0, 1);
		glm::vec3 top3 = glm::vec3(top[0], top[1], top[2]);
		glm::vec3 axis = top3 - position3;
		return axis;
	}

	void onlyRotateCustomModel(float rot_speed, glm::vec3 axis) {
		if (this->customModel != nullptr) {
			glm::vec3 position = getPosition();
			glm::vec3 invPosition = position * (-1.0f);
			this->customModel->rotateAroundAxis(rot_speed, axis, position);
		}
	}

	void rotate(float rot_speed) {

		glm::vec3 position = getPosition();
		glm::vec3 invPosition = position * (-1.0f);
		translate(invPosition);
		glm::vec3 axis = getAxis();
		sphere->model = glm::myRotate(sphere->model, rot_speed, axis);
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

	void rotateAroundAxis(float rot_speed, glm::vec3 axis, glm::vec3 positionOfAxis) {
		glm::vec3 invPosition = positionOfAxis * (-1.0f);
		translate(invPosition);
		sphere->model = glm::myRotate(sphere->model, rot_speed, axis);
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

	void tilt(float angle) {
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

	void scale(float factor) {
		sphere->model = glm::scale(sphere->model, glm::vec3(factor));
	}

	void addChild(Orb* child) {
		childs.push_back(child);
		child->parent = this;
	}

	void addOrbit(float size, const glm::vec3& color) {
		glm::vec3 position = getPosition();
		orbits.push_back(new Orbit(size, color, position));
	}

	void addAxis(float size, const glm::vec3& color) {
		if (axis == nullptr) {
			glm::vec3 position = getPosition();
			axis = new Axis(size, color, position);
		}
	}

	void addCustomModel(char *path, float size, const glm::vec3& color) {
		if (customModel == nullptr) {
			glm::vec3 position = getPosition();
			customModel = new Model(path, size, color, position, false);
			customModel->runTests();
			customModel->showStatistics();
		}
	}

	void render(const glm::mat4& view, const glm::mat4& projection, const bool bbox, const bool hnorms, const bool norms) {
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

public:
	// Only public for easy access.
	Sphere* sphere;
private:
	std::vector<Orb*> childs;
	std::vector<Orbit*> orbits;
	Axis* axis;
	Model* customModel;
	Orb* parent;
};
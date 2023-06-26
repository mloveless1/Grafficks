#pragma once
#include <vector>
#include <types.h>
#include <glad/glad.h>
#include <Texture.h>

class Mesh{
public:
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, int shapeType);
	Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, const glm::vec3& color, int shapeType);
	Mesh(std::vector<Vertex>& vertices, GLuint size, int shapeType);

	std::vector<glm::vec3> _torusVertices;
	std::vector<Vertex> _vertices{};
	std::vector<uint32_t> _indices{};

	void Draw();
	glm::mat4 Transform{ 1.0f };

private:
	GLuint _nVertices {0};
	void init(std::vector<Vertex>& vertices, std::vector<uint32_t>& elements, int shapeType);
	uint32_t _elementCount {0};
	GLuint _vertexBufferObject{};
	GLuint _shaderProgram{};
	GLuint _vertexArrayObject{};
	GLuint _elementBufferObject{};
	int _shapeType{ 0 }; 

public:
	GLuint _textureID;
};


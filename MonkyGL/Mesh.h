#pragma once
#include <vector>
#include <glm.hpp>
#include "ObjLoader.h"
class Shader;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);
	Mesh(Gorilla::ObjData someData);

	~Mesh();
	void Draw(Shader* aShader);

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	int IndicesSize = 0;

protected:
	unsigned int VBO, VAO, EBO;

};



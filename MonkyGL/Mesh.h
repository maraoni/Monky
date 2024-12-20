#pragma once
#include <vector>
#include <glm.hpp>
#include "ObjLoader.h"
#include <memory>

class Shader;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

class Mesh
{
public:
	Mesh();
	Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
	Mesh(const Gorilla::ObjData&);

	~Mesh();
	void Draw(std::shared_ptr<Shader> aShader);

private:
	std::vector<Vertex> myVertices;
	std::vector<unsigned int> myIndices;
	int IndicesSize = 0;

protected:
	unsigned int VBO, VAO, EBO;

};



#pragma once
class Shader;

class Mesh
{
public:
	Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);
	~Mesh();
	void Draw(Shader* aShader);

protected:
	unsigned int VBO, VAO, EBO;
};



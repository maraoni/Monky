#pragma once
class Shader;
class Texture;

class Mesh
{
public:
	Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize);
	~Mesh();
	void Draw(Shader* aShader);

	void ApplyTexture(Texture* aTexture);
protected:
	unsigned int VBO, VAO, EBO;
	Texture* myTexture;
};



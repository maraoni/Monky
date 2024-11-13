#include "Mesh.h"
#include <glad/glad.h>
#include <glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include <iostream>

Mesh::Mesh(const float* someVertices, size_t aVertexSize, unsigned int* someIndices, size_t aIndexSize)
{
	IndicesSize = 0;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, aVertexSize, someVertices, GL_STATIC_DRAW);

	EBO = 0;
	if (someIndices && aIndexSize > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, aIndexSize, someIndices, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh(Gorilla::ObjData someData)
{
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    std::vector<float> vertexData;
    for (size_t i = 0; i < someData.vertices.size(); ++i)
    {
        vertexData.push_back(someData.vertices[i].x);
        vertexData.push_back(someData.vertices[i].y);
        vertexData.push_back(someData.vertices[i].z);

        if (i < someData.texCoords.size())
        {
            vertexData.push_back(someData.texCoords[i].x);
            vertexData.push_back(someData.texCoords[i].y);
        }
        else 
        {
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(0.0f); // Default value
        }

        // Normal
        if (i < someData.normals.size())
        {
            vertexData.push_back(someData.normals[i].x);
            vertexData.push_back(someData.normals[i].y);
            vertexData.push_back(someData.normals[i].z);
        }
        else 
        {
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(1.0f); // Default normal pointing up
        }
    }

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, someData.indices.size() * sizeof(unsigned int), someData.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    IndicesSize = someData.indices.size();
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader* aShader)
{
	aShader->Use();
	glBindVertexArray(VAO);

	if (IndicesSize > 0)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glDrawElements(GL_TRIANGLES, IndicesSize, GL_UNSIGNED_INT, (void*)0);

		glBindVertexArray(0);
	}

	if (EBO == 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
	else
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	glBindVertexArray(0);
}

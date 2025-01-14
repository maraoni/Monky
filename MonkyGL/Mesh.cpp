#include "Mesh.h"
#include <glad/glad.h>
#include <glfw3.h>
#include "Texture.h"
#include "Shader.h"
#include <iostream>


Mesh::Mesh()
{

}

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)
{
    // Set initial index size
    IndicesSize = indices.size();

    // Create and bind VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create and bind VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // If indices are provided, set up the Element Buffer Object (EBO)
    if (IndicesSize > 0)
    {
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    }

    // Define vertex attributes for the position (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Define vertex attributes for texture coordinates (2 floats)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind buffers and VAO to clean up
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::Mesh(const Gorilla::ObjData& objData)
{
    // Create and bind VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Prepare a vector to hold all vertex data (position + texture + normal)
    std::vector<float> vertexData;
    size_t vertexCount = objData.vertices.size();

    // Loop over vertices and create the interleaved vertex data
    for (size_t i = 0; i < vertexCount; ++i)
    {
        // Add position
        vertexData.push_back(objData.vertices[i].x);
        vertexData.push_back(objData.vertices[i].y);
        vertexData.push_back(objData.vertices[i].z);

        // Add texture coordinates, or use defaults if not available
        if (i < objData.texCoords.size())
        {
            vertexData.push_back(objData.texCoords[i].x);
            vertexData.push_back(objData.texCoords[i].y);
        }
        else
        {
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(0.0f); // Default value
        }

        // Add normal vectors, or use defaults if not available
        if (i < objData.normals.size())
        {
            vertexData.push_back(objData.normals[i].x);
            vertexData.push_back(objData.normals[i].y);
            vertexData.push_back(objData.normals[i].z);
        }
        else
        {
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(0.0f); // Default value
            vertexData.push_back(1.0f); // Default normal (pointing upwards)
        }
    }

    // Create and bind VBO (Vertex Buffer Object)
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers (Position, Texture, Normal)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // TexCoords
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float))); // Normal
    glEnableVertexAttribArray(2);

    // Create and bind EBO (Element Buffer Object) for indices
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, objData.indices.size() * sizeof(unsigned int), objData.indices.data(), GL_STATIC_DRAW);

    // Unbind VAO to clean up
    glBindVertexArray(0);

    // Set index size for later use in drawing
    IndicesSize = objData.indices.size();
}

Mesh::~Mesh()
{
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
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

	//if (EBO == 0)
	//{
	//	glDrawArrays(GL_TRIANGLES, 0, 3);
	//}
	//else
	//{
	//	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	//}

	//glBindVertexArray(0);
}

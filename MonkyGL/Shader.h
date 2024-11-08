#pragma once
#include <string>
#include <glm.hpp>

class Texture;

class Shader
{
private:
	std::string LoadShader(const char* aPath);
	unsigned int LoadVertexShader(const char* aPath);
	unsigned int LoadFragmentShader(const char* aPath);
	void ApplyTexture(Texture* aTexture);
public:
	Shader(const char* aVertexPath, const char* aFragmentPath);

	void SetMatrix4(glm::mat4 aMatrix, const std::string& aName);

	void SetVector3(glm::vec3 aVector3, const std::string& aName);  // not implemented
	void SetVector2(glm::vec2 aVector2, const std::string& aName);  // not implemented
	void SetFloat(float aFloat, const std::string& aName);			// not implemented
	void SetInt(int aInt, const std::string& aName);				// not implemented

	unsigned int myShaderProgram;
	void Use();
};


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

public:
	Shader(const char* aVertexPath, const char* aFragmentPath);
	~Shader();

	bool RecompileShader(const char* aVertexPath, const char* aFragmentPath);

	void SetMatrix4(glm::mat4 aMatrix, const std::string& aName);

	void SetVector4(glm::vec4 aVector4, const std::string& aName);  
	void SetVector3(glm::vec3 aVector3, const std::string& aName);  
	void SetVector2(glm::vec2 aVector2, const std::string& aName);  
	void SetFloat(float aFloat, const std::string& aName);			
	void SetInt(int aInt, const std::string& aName);				

	unsigned int myShaderProgram;
	void Use();
};


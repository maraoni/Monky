#include "Shader.h"
#include <iostream>
#include <glad/glad.h>
#include <glfw3.h>
#include <fstream>
#include <sstream>
#include <fwd.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>

Shader::Shader(const char* aVertexPath, const char* aFragmentPath)
{
	if (!RecompileShader(aVertexPath, aFragmentPath))
	{
		return;
	}
}

Shader::~Shader()
{
	int kek = 0;
}

bool Shader::RecompileShader(const char* aVertexPath, const char* aFragmentPath)
{
	int result;
	char Log[512];

	unsigned int VertexShader = LoadVertexShader(aVertexPath);
	unsigned int FragmentShader = LoadFragmentShader(aFragmentPath);

	myShaderProgram = glCreateProgram();
	glAttachShader(myShaderProgram, VertexShader);
	glAttachShader(myShaderProgram, FragmentShader);
	glLinkProgram(myShaderProgram);

	glGetProgramiv(myShaderProgram, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(myShaderProgram, 512, NULL, Log);
		std::cout << "Shader program could not compile\n" << Log << std::endl;

		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);

		myShaderProgram = 0;

		return false;
	}

	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	return true;
}

void Shader::Use()
{
	glUseProgram(myShaderProgram);
}

std::string Shader::LoadShader(const char* aPath)
{
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(aPath);

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();

		shaderFile.close();
		shaderCode = shaderStream.str();

		return shaderCode;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Could not load shader file from path - " << aPath << "\n";
		return "";
	}
}

unsigned int Shader::LoadVertexShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;

	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile vertex shader \n" << Log << std::endl;
	}

	return shaderObject;
}

unsigned int Shader::LoadFragmentShader(const char* aPath)
{
	int result;
	char Log[512];

	std::string shaderCodeString = LoadShader(aPath);
	const char* shaderCode = shaderCodeString.c_str();

	unsigned int shaderObject;

	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderObject, 1, &shaderCode, NULL);
	glCompileShader(shaderObject);

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

	if (!result)
	{
		glGetShaderInfoLog(shaderObject, 512, NULL, Log);
		std::cout << "Failed to compile fragment shader \n" << Log << std::endl;
	}

	return shaderObject;
}

void Shader::SetMatrix4(glm::mat4 aMatrix, std::string aName)
{
	glUniformMatrix4fv(glGetUniformLocation(myShaderProgram, aName.c_str()), 1, GL_FALSE, glm::value_ptr(aMatrix));
}

void Shader::SetVector4(glm::vec4 aVector4, std::string aName)
{
	glUniform4f(glGetUniformLocation(myShaderProgram, aName.c_str()), aVector4.x, aVector4.y, aVector4.z, aVector4.w);
}

void Shader::SetVector3(glm::vec3 aVector3, std::string aName)
{

}

void Shader::SetVector2(glm::vec2 aVector2, std::string aName)
{

}

void Shader::SetFloat(float aFloat, std::string aName)
{
	glUniform1f(glGetUniformLocation(myShaderProgram, aName.c_str()), aFloat);
}

void Shader::SetInt(int aInt, std::string aName)
{

}

#include "../include/shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vShaderCode, fShaderCode;
	std::ifstream vShaderFile, fShaderFile;
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		std::stringstream vBuffer, fBuffer;
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		vBuffer << vShaderFile.rdbuf();
		fBuffer << fShaderFile.rdbuf();
		vShaderFile.close();
		fShaderFile.close();
		vShaderCode = vBuffer.str();
		fShaderCode = fBuffer.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Failed to open/read file:\n" << e.what() << std::endl;
	}

	const char* vCode = vShaderCode.c_str();
	const char* fCode = fShaderCode.c_str();

	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);
	checkError(vertex);

	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);
	checkError(fragment);

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	checkError(ID, PROGRAM);
}

void Shader::use()
{
	glUseProgram(ID);
}

unsigned int Shader::getID()
{
	return ID;
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::checkError(unsigned int id, ERROR_TYPE type)
{
	int success;
	char infoLog[512];

	if (type == SHADER) {
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(id, 512, NULL, infoLog);
			std::cout << "SHADER ERROR: " << infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(id, 512, NULL, infoLog);
			std::cout << "PROGRAM ERROR: " << infoLog << std::endl;
		}
	}
}

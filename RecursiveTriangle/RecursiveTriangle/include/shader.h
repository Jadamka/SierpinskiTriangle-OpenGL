#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

enum ERROR_TYPE
{
	SHADER,
	PROGRAM
};

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	void use();
	unsigned int getID();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
private:
	unsigned int ID;
	void checkError(unsigned int id, ERROR_TYPE type = SHADER);
};

#endif
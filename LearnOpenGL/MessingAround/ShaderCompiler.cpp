#include "ShaderCompiler.h"

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

ShaderCompiler::ShaderCompiler(const char* vertexPath, const char* fragmentPath) {

//////// Retrieve the shade code from the files ///////////////////////////////

	std::ifstream vertexFile;
	std::ifstream fragmentFile;
	std::string vertexCode;
	std::string fragmentCode;

	// Ensure ifstream objects can throw exceptions.
	vertexFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vertexFile.open(vertexPath);
		fragmentFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// read file’s buffer contents into streams
		vShaderStream << vertexFile.rdbuf();
		fShaderStream << fragmentFile.rdbuf();

		// close file handlers
		vertexFile.close();
		fragmentFile.close();

		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

	} catch (std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

//////// Compile the shaders //////////////////////////////////////////////////

	unsigned int vertexShaderID, fragmentShaderID;
	int success;
	char infoLog[512];

	// vertex Shader
	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
	glCompileShader(vertexShaderID);

	// print compile errors if any
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};
	
	// fragment shader
	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShaderID);

	// print compile errors if any
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

//////// Linking the shaders //////////////////////////////////////////////////

	// Shader Program
	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShaderID);
	glAttachShader(ShaderProgramID, fragmentShaderID);
	glLinkProgram(ShaderProgramID);

	// print linking errors if any
	glGetProgramiv(ShaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(ShaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete shaders; they’re linked into the program and no longer necessary.
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

}

void ShaderCompiler::useProgram() {
	glUseProgram(ShaderProgramID);
}

void ShaderCompiler::deleteProgram() {
	glDeleteProgram(ShaderProgramID);
}

void ShaderCompiler::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}
void ShaderCompiler::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void ShaderCompiler::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}

#ifndef SHADER_H
#define SHADER_H

#include <string>

class ShaderCompiler {

public:

	// the program ID
	unsigned int ShaderProgramID;

	// constructor reads and builds the shader
	ShaderCompiler(const char* vertexPath, const char* fragmentPath);

	// use/activate the shader
	void use();

	// utility uniform functions
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};

#endif
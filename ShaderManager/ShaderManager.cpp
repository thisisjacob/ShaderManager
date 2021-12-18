#include "ShaderManager.h"
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>

ShaderManager::ShaderManager() {
	vertexShaderId = -1;
	fragmentShaderId = -1;
	activeProgramId = -1;
}

unsigned int ShaderManager::GetUniformId(std::string uniformName) {
	if (activeProgramId < 0) {
		std::cerr << "ShaderManager.ModifyUniform Failed: No program active.\n";
		return -1;
	}
	auto uniformLocation = glGetUniformLocation(activeProgramId, uniformName.c_str());
	if (uniformLocation < 0) {
		std::cerr << "ShaderManager.ModifyUniform Failed: Invalid uniform provided. " << uniformName << "\n";
		return -1;
	}
	return uniformLocation;
}

bool ShaderManager::AddShader(std::string filePath, unsigned int shaderType) {
	// If there is no matching shader enum, then this function fails
	if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER) {
		std::cerr << "ShaderManager.AddShader Failed: Invalid shaderType specified. Must be an OpenGL shader enum.\n";
		return false;
	}
	// Read from the file
	std::fstream reader;
	std::stringstream buffer;
	reader.open(filePath);
	// Check for reading failure
	if (!reader) {
		std::cerr << "ShaderManager.AddShader Failed: Error when reading shader from file " << filePath << "\n";
		return false;
	}
	buffer << reader.rdbuf();
	// Prepare shader
	unsigned int shader = glCreateShader(shaderType);
	if (shaderType == GL_VERTEX_SHADER) { vertexShaderId = shader; }
	if (shaderType == GL_FRAGMENT_SHADER) { fragmentShaderId = shader; }
	std::string shaderString = buffer.str();
	const char* cString = shaderString.c_str();
	reader.close();
	glShaderSource(shader, 1, &cString, NULL);
	glCompileShader(shader);
	// Check for compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ShaderManager.AddShader Failed: Compilation error. " << infoLog << "\n";
		return false;
	}
	return true;
}

bool ShaderManager::BuildProgram(std::string newName) {
	// Must at least have a vertex and fragment shader
	if (vertexShaderId == -1 || fragmentShaderId == -1) {
		std::cerr << "ShaderManager.BuildProgram Failed: No Vertex or Fragment shader compiled.\n";
		return false;
	}
	// Program with given name already exists
	if (programs.find(newName) != programs.end()) {
		std::cerr << "ShaderManager.BuildProgram Failed: Provided newName of program already associated with another program.\n";
		return false;
	}
	unsigned int newProgram = glCreateProgram();
	glAttachShader(newProgram, vertexShaderId);
	glAttachShader(newProgram, fragmentShaderId);
	glLinkProgram(newProgram);
	programs.insert({ newName, newProgram });
	// TODO: Check link validity
	// Should reset shader information if failed

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	vertexShaderId = -1;
	fragmentShaderId = -1;
}

bool ShaderManager::UseProgram(std::string programName) {
	auto program = programs.find(programName);
	// Program not found
	if (program == programs.end()) {
		std::cerr << "ShaderManager.UseProgram Failed: No program of programName found.\n";
		return false;
	}
	// Enable program
	glUseProgram(program->second);
	activeProgramId = program->second;
	return true;
}

unsigned int ShaderManager::GetProgram(std::string programName) {
	auto program = programs.find(programName);
	if (program == programs.end()) {
		std::cerr << "ShaderManager.GetProgram Failed: No program of programName found.\n";
		return false;
	}
	return program->second;
}

bool ShaderManager::ModifyUniform(std::string uniformName, float x, float y, float z, float w) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform4f(uniformVal, x, y, z, w);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, float x, float y, float z) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform3f(uniformVal, x, y, z);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, float x, float y) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform2f(uniformVal, x, y);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, float val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform1f(uniformVal, val);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, unsigned int x) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform1i(uniformVal, x);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, unsigned int x, unsigned int y) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform2i(uniformVal, x, y);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, unsigned int x, unsigned int y, unsigned int z) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform3i(uniformVal, x, y, z);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, unsigned int x, unsigned int y, unsigned int z, unsigned int w) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// TODO: Error check
	glUniform4i(uniformVal, x, y, z, w);
	return true;
}
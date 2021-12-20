#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>

class ShaderManager {
private:
	int vertexShaderId;
	int fragmentShaderId;
	int activeProgramId;
	std::unordered_map<std::string, unsigned int> programs;
	unsigned int GetUniformId(std::string uniformName);
public:
	ShaderManager();
	bool AddShader(std::string filePath, unsigned int shaderType);
	bool BuildProgram(std::string newName);
	bool UseProgram(std::string programName);
	unsigned int GetProgram(std::string programName);
	bool ModifyUniform(std::string uniformName, std::initializer_list<int> args);
	bool ModifyUniform(std::string uniformName, std::initializer_list<unsigned int> args);
	bool ModifyUniform(std::string uniformName, std::initializer_list<float> args);
	bool ModifyUniform(std::string uniformName, std::initializer_list<double> args);
	bool ModifyUniform(std::string uniformName, int val);
	bool ModifyUniform(std::string uniformName, unsigned int val);
	bool ModifyUniform(std::string uniformName, float val);
	bool ModifyUniform(std::string uniformName, double val);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, bool trans);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, int numMatrices, bool trans);

};
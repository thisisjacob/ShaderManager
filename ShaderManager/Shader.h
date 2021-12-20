#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>


class Shader {
private:
	int vertexShaderId;
	int fragmentShaderId;
	int programId;
	bool isBuilt;
public:
	Shader();
	bool AddShader(std::string filePath, unsigned int shaderType);
	bool BuildProgram(std::string newName);
	bool UseProgram(std::string programName);
	unsigned int GetUniformId(std::string uniformName);
	template <typename T>
	bool ModifyUniform(std::string uniformName, const T* array, int numVecs, int vecSize);
	bool ModifyUniform(std::string uniformName, int val);
	bool ModifyUniform(std::string uniformName, unsigned int val);
	bool ModifyUniform(std::string uniformName, float val);
	bool ModifyUniform(std::string uniformName, double val);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, bool trans);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, int numMatrices, bool trans);

};
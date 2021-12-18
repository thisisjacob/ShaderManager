#pragma once
#include <unordered_map>

class ShaderManager {
private:
	int vertexShaderId;
	int fragmentShaderId;
	int activeProgramId;
	std::unordered_map<std::string, unsigned int> programs;
public:
	ShaderManager();
	bool AddShader(std::string filePath, unsigned int shaderType);
	bool BuildProgram(std::string newName);
	bool UseProgram(std::string programName);
	unsigned int GetProgram(std::string programName);
	bool ModifyUniform(std::string uniformName, float x, float y, float z, float w);
};
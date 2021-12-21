#include "Shader.h"

Shader::Shader() {
	vertexShaderId = -1;
	fragmentShaderId = -1;
	programId = -1;
	isBuilt = false;
}

unsigned int Shader::GetUniformId(std::string uniformName) {
	if (programId < 0) {
		std::cerr << "Shader.ModifyUniform Failed: Program associated with Shader object not built.\n";
		return -1;
	}
	auto uniformLocation = glGetUniformLocation(programId, uniformName.c_str());
	if (uniformLocation < 0) {
		std::cerr << "Shader.ModifyUniform Failed: Invalid uniform provided. " << uniformName << "\n";
		return -1;
	}
	return uniformLocation;
}

bool Shader::AddShader(std::string filePath, unsigned int shaderType) {
	// No functionality for changing shaders after program is built, so disallow
	if (isBuilt)
		return false;
	// If there is no matching shader enum, then this function fails
	if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER) {
		std::cerr << "Shader.AddShader Failed: Invalid shaderType specified. Must be an OpenGL shader enum.\n";
		return false;
	}
	// Read from the file
	std::fstream reader;
	std::stringstream buffer;
	reader.open(filePath);
	// Check for reading failure
	if (!reader) {
		std::cerr << "Shader.AddShader Failed: Error when reading shader from file " << filePath << "\n";
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
		std::cout << "Shader.AddShader Failed: Compilation error. " << infoLog << "\n";
		return false;
	}
	return true;
}

bool Shader::BuildProgram(std::string newName) {
	// Must at least have a vertex and fragment shader
	if (vertexShaderId == -1 || fragmentShaderId == -1) {
		std::cerr << "Shader.BuildProgram Failed: No Vertex or Fragment shader compiled.\n";
		return false;
	}
	unsigned int newProgram = glCreateProgram();
	programId = newProgram;
	glAttachShader(newProgram, vertexShaderId);
	glAttachShader(newProgram, fragmentShaderId);
	glLinkProgram(newProgram);
	// TODO: Check link validity
	// Should reset shader information if failed

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragmentShaderId);
	vertexShaderId = -1;
	fragmentShaderId = -1;

	return true;
}

bool Shader::UseProgram(std::string programName) {
	// Enable program
	glUseProgram(programId);
	// TODO: Check for errors
	return true;
}

bool Shader::ModifyUniform(std::string uniformName, int val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1i(uniformVal, val);
	return true;
}

bool Shader::ModifyUniform(std::string uniformName, unsigned int val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1ui(uniformVal, val);
	return true;
}

bool Shader::ModifyUniform(std::string uniformName, float val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1f(uniformVal, val);
	return true;
}
bool Shader::ModifyUniform(std::string uniformName, double val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1d(uniformVal, val);
	return true;
}

bool Shader::ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, bool trans) {
	return ModifyUniform(uniformName, rows, cols, matrix, 1, trans);
}

bool Shader::ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, int numMatrices, bool trans) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	// Determine which uniform function to use
	auto transEnum = trans ? GL_TRUE : GL_FALSE;
	if (rows == 2 && cols == 2)
		glUniformMatrix2fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 3 && cols == 3)
		glUniformMatrix3fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 4 && cols == 4)
		glUniformMatrix4fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 2 && cols == 3)
		glUniformMatrix2x3fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 3 && cols == 2)
		glUniformMatrix3x2fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 2 && cols == 4)
		glUniformMatrix2x4fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 4 && cols == 2)
		glUniformMatrix4x2fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 3 && cols == 4)
		glUniformMatrix3x4fv(uniformVal, numMatrices, transEnum, matrix);
	else if (rows == 4 && cols == 3)
		glUniformMatrix4x3fv(uniformVal, numMatrices, transEnum, matrix);
	else {
		std::cerr << "Shader::ModifyUniform(std::string, int, int, const glm::f32*) failed. Invalid number of rows and columns provided.";
		return false;
	}
	// TODO: check errors with glGetError()
	return true;
}
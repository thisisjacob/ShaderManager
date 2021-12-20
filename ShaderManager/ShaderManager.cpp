#include "ShaderManager.h"

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

bool ShaderManager::ModifyUniform(std::string uniformName, std::initializer_list<int> args) {
	int numArgs = args.size();
	if (numArgs == 0 || numArgs > 4) {
		std::cout << "ShaderManager.ModifyUniform failed. Invalid number of arguments: " << numArgs << "\n";
		return false;
	}
	int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;

	const int* firstElement = args.begin();
	if (numArgs == 1)
		glUniform1iv(uniformVal, 1, firstElement);
	if (numArgs == 2)
		glUniform2iv(uniformVal, 1, firstElement);
	if (numArgs == 3)
		glUniform3iv(uniformVal, 1, firstElement);
	if (numArgs == 4)
		glUniform4iv(uniformVal, 1, firstElement);

	auto err = glGetError();
}

bool ShaderManager::ModifyUniform(std::string uniformName, std::initializer_list<float> args) {
	int numArgs = args.size();
	if (numArgs == 0 || numArgs > 4) {
		std::cout << "ShaderManager.ModifyUniform failed. Invalid number of arguments: " << numArgs << "\n";
		return false;
	}
	int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;

	const float* firstElement = args.begin();
	if (numArgs == 1)
		glUniform1fv(uniformVal, 1, firstElement);
	if (numArgs == 2)
		glUniform2fv(uniformVal, 1, firstElement);
	if (numArgs == 3)
		glUniform3fv(uniformVal, 1, firstElement);
	if (numArgs == 4)
		glUniform4fv(uniformVal, 1, firstElement);

	auto err = glGetError();
}

bool ShaderManager::ModifyUniform(std::string uniformName, std::initializer_list<unsigned int> args) {
	int numArgs = args.size();
	if (numArgs == 0 || numArgs > 4) {
		std::cout << "ShaderManager.ModifyUniform failed. Invalid number of arguments: " << numArgs << "\n";
		return false;
	}
	int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;

	const unsigned int* firstElement = args.begin();
	if (numArgs == 1)
		glUniform1uiv(uniformVal, 1, firstElement);
	if (numArgs == 2)
		glUniform2uiv(uniformVal, 1, firstElement);
	if (numArgs == 3)
		glUniform3uiv(uniformVal, 1, firstElement);
	if (numArgs == 4)
		glUniform4uiv(uniformVal, 1, firstElement);

	auto err = glGetError();
}

bool ShaderManager::ModifyUniform(std::string uniformName, std::initializer_list<double> args) {
	int numArgs = args.size();
	if (numArgs == 0 || numArgs > 4) {
		std::cout << "ShaderManager.ModifyUniform failed. Invalid number of arguments: " << numArgs << "\n";
		return false;
	}
	int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;

	const double* firstElement = args.begin();
	if (numArgs == 1)
		glUniform1dv(uniformVal, 1, firstElement);
	if (numArgs == 2)
		glUniform2dv(uniformVal, 1, firstElement);
	if (numArgs == 3)
		glUniform3dv(uniformVal, 1, firstElement);
	if (numArgs == 4)
		glUniform4dv(uniformVal, 1, firstElement);

	auto err = glGetError();
}

bool ShaderManager::ModifyUniform(std::string uniformName, int val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1i(uniformVal, val);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, unsigned int val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1ui(uniformVal, val);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, float val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1f(uniformVal, val);
	return true;
}
bool ShaderManager::ModifyUniform(std::string uniformName, double val) {
	unsigned int uniformVal = GetUniformId(uniformName);
	if (uniformVal < 0) return false;
	glUniform1d(uniformVal, val);
	return true;
}

bool ShaderManager::ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, bool trans) {
	return ModifyUniform(uniformName, rows, cols, matrix, 1, trans);
}

bool ShaderManager::ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, int numMatrices, bool trans) {
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
		std::cerr << "ShaderManager::ModifyUniform(std::string, int, int, const glm::f32*) failed. Invalid number of rows and columns provided.";
		return false;
	}
	// TODO: check errors with glGetError()
	return true;
}
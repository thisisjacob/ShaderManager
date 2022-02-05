#pragma once
#include <unordered_map>
#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>


class Shader {
private:
	std::unordered_map<unsigned int, unsigned int> shaderTypeIds;
	int programId;
	bool isBuilt;
public:
	Shader();
	bool AddShader(std::string filePath, unsigned int shaderType);
	bool BuildProgram();
	bool UseProgram();
	unsigned int GetUniformId(std::string uniformName);
	template <typename T>
	bool ModifyUniform(std::string uniformName, const T* array, int numVecs, int vecSize) {
		// Ensure that the provided data is numeric, and that the vector size is valid
		static_assert(std::is_same<T, int>::value || std::is_same<T, unsigned int>::value || std::is_same<T, float>::value || std::is_same<T, double>::value,
						"Shader::ModifyUniform(std::string, const T*, int, int) failed. Non numeric type passed.\n");
		if (vecSize > 4 || vecSize <= 0) {
			std::cerr << "Shader::ModifyUniform(std::string, const T*, int, int) failed. vecSize invalid: must be greater than 0, or less than or equal to 4.\n";
			return false;
		}

		// Get uniform location, validate
		int uniformVal = GetUniformId(uniformName);
		if (uniformVal < 0) return false;

		if constexpr (std::is_same<T, int>::value) {
			if (vecSize == 1)
				glUniform1iv(uniformVal, numVecs, array);
			else if (vecSize == 2)
				glUniform2iv(uniformVal, numVecs, array);
			else if (vecSize == 3)
				glUniform3iv(uniformVal, numVecs, array);
			else if (vecSize == 4)
				glUniform4iv(uniformVal, numVecs, array);
		}
		else if constexpr (std::is_same<T, unsigned int>::value) {
			if (vecSize == 1)
				glUniform1uiv(uniformVal, numVecs, array);
			else if (vecSize == 2)
				glUniform2uiv(uniformVal, numVecs, array);
			else if (vecSize == 3)
				glUniform3uiv(uniformVal, numVecs, array);
			else if (vecSize == 4)
				glUniform4uiv(uniformVal, numVecs, array);
		}
		else if constexpr (std::is_same<T, float>::value) {
			if (vecSize == 1)
				glUniform1fv(uniformVal, numVecs, array);
			else if (vecSize == 2)
				glUniform2fv(uniformVal, numVecs, array);
			else if (vecSize == 3)
				glUniform3fv(uniformVal, numVecs, array);
			else if (vecSize == 4)
				glUniform4fv(uniformVal, numVecs, array);
		}
		else if constexpr (std::is_same<T, double>::value) {
			if (vecSize == 1)
				glUniform1dv(uniformVal, numVecs, array);
			else if (vecSize == 2)
				glUniform2dv(uniformVal, numVecs, array);
			else if (vecSize == 3)
				glUniform3dv(uniformVal, numVecs, array);
			else if (vecSize == 4)
				glUniform4dv(uniformVal, numVecs, array);
		}

		// Check errors
		auto err = glGetError();
		return true;
	}

	bool ModifyUniform(std::string uniformName, int val);
	bool ModifyUniform(std::string uniformName, unsigned int val);
	bool ModifyUniform(std::string uniformName, float val);
	bool ModifyUniform(std::string uniformName, double val);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, bool trans);
	bool ModifyUniform(std::string uniformName, int rows, int cols, const glm::f32* matrix, int numMatrices, bool trans);
};
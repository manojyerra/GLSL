#ifndef ShaderProgram_H
#define ShaderProgram_H


#include "GLInclude.h"
#include <string>
#include <vector>

class ShaderProgram
{
private:
	GLuint _programID;
	std::string _vertexShaderPath;
	std::string _geometryShaderPath;
	std::string _fragmentShaderPath;

	bool _hasGeometryShader;

	GLint CompileShader(const char* shaderFilePath, GLenum shaderType);

public:
	ShaderProgram(std::string vertexShaderFilePath, std::string fragmentShaderFilePath);
	ShaderProgram(std::string vertexShaderFilePath, std::string geometryShaderFilePath, std::string fragmentShaderFilePath);

	~ShaderProgram();

	GLuint ProgramID();

	void Begin();
	void End();

	void SetUniformMatrix4fv(const char* str, const float* mat);
	void SetUniformMatrix3fv(const char* str, const float* mat);

	void SetUniform4f(const char* str, float v1, float v2, float v3, float v4);
	void SetUniform3f(const char* str, float v1, float v2, float v3);
	void SetUniform2f(const char* str, float v1, float v2);
	void SetUniform1f(const char* str, float val);
	void SetUniform1i(const char* str, int val);

	std::string GetVertexShaderFilePath();
	std::string GetGeometryShaderFilePath();
	std::string GetFragmentShaderFilePath();

	bool HasGeometryShader();
};

#endif

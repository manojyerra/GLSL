#ifndef ShaderProgram_H
#define ShaderProgram_H

#include <GL/glew.h>

#include <string>
#include <vector>
using namespace std;

class ShaderProgram
{
private:
	GLuint _programID;
	string _vertexShaderPath;
	string _geometryShaderPath;
	string _fragmentShaderPath;

	GLint CompileShader(const char* shaderFilePath, GLenum shaderType);

public:
	ShaderProgram(string vertexShaderFilePath, string fragmentShaderFilePath);
	ShaderProgram(string vertexShaderFilePath, string geometryShaderFilePath, string fragmentShaderFilePath);

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

	string GetVertexShaderFilePath();
	string GetGeometryShaderFilePath();
	string GetFragmentShaderFilePath();
};

#endif

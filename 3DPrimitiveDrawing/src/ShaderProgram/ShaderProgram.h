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

	void SetUniformMatrix4fv(const char* str, float* mat);
	void SetUniform1f(const char* str, float val);

	string GetVertexShaderFilePath();
	string GetGeometryShaderFilePath();
	string GetFragmentShaderFilePath();
};

#endif

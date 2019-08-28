#ifndef ShaderProgram_H
#define ShaderProgram_H


#include "GLInclude.h"
#include <string>
#include <vector>
#include "glm/ext.hpp"

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

	void SetUniformMatrix4fv(const char* str, glm::mat4 mat);
	void SetUniformMatrix3fv(const char* str, glm::mat3 mat);

	void SetUniform4f(const char* str, float v1, float v2, float v3, float v4);
	void SetUniform3f(const char* str, float v1, float v2, float v3);
	void SetUniform2f(const char* str, float v1, float v2);
	void SetUniform1f(const char* str, float val);
	void SetUniform1i(const char* str, int val);

	void SetUniform4f(const char* str, glm::vec4 vec);
	void SetUniform3f(const char* str, glm::vec3 vec);
	void SetUniform2f(const char* str, glm::vec2 vec);


	void SetUniformMatrix4fv(GLint loc, const float* mat);
	void SetUniformMatrix3fv(GLint loc, const float* mat);

	void SetUniformMatrix4fv(GLint loc, glm::mat4 mat);
	void SetUniformMatrix3fv(GLint loc, glm::mat3 mat);

	void SetUniform4f(GLint loc, float v1, float v2, float v3, float v4);
	void SetUniform3f(GLint loc, float v1, float v2, float v3);
	void SetUniform2f(GLint loc, float v1, float v2);
	void SetUniform1f(GLint loc, float val);
	void SetUniform1i(GLint loc, int val);

	void SetUniform4f(GLint loc, glm::vec4 vec);
	void SetUniform3f(GLint loc, glm::vec3 vec);
	void SetUniform2f(GLint loc, glm::vec2 vec);


	GLint GetUniformLocation(const char* str);
	GLint GetAttribLocation(const char* str);

	std::string GetVertexShaderFilePath();
	std::string GetGeometryShaderFilePath();
	std::string GetFragmentShaderFilePath();

	bool HasGeometryShader();
};

#endif

#include "ShaderProgram.h"
#include "FileReader.h"
#include "Platform.h"

ShaderProgram::ShaderProgram(std::string vertexShaderFilePath, std::string fragmentShaderFilePath)
{
	_programID = 0;
	_vertexShaderPath = vertexShaderFilePath;
	_geometryShaderPath = "";
	_fragmentShaderPath = fragmentShaderFilePath;

	_hasGeometryShader = false;

	Platform::debugPrint("\nCompiling vertex shader. FilePath : %s\n", vertexShaderFilePath.c_str());
	GLint vertexShaderObj = CompileShader(_vertexShaderPath.c_str(), GL_VERTEX_SHADER);
	Platform::debugPrint("Compiling fragment shader. FilePath : %s\n", fragmentShaderFilePath.c_str());
	GLint fragmentShaderObj = CompileShader(_fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);

	Platform::debugPrint("Linking program...");
	_programID = glCreateProgram();
	glAttachShader(_programID, vertexShaderObj);
	glAttachShader(_programID, fragmentShaderObj);
	glLinkProgram(_programID);

	GLint linkStatus = GL_FALSE;
	int infoLogLength = 0;

	glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	glDetachShader(_programID, vertexShaderObj);
	glDetachShader(_programID, fragmentShaderObj);

	glDeleteShader(vertexShaderObj);
	glDeleteShader(fragmentShaderObj);

	if (infoLogLength > 0)
	{
		std::vector<char> programInfoLog(infoLogLength + 1);
		glGetProgramInfoLog(_programID, infoLogLength, NULL, &programInfoLog[0]);
		Platform::debugPrint("%s\n", &programInfoLog[0]);
	}

	if (!linkStatus)
	{
		throw std::exception("Linking failed...");
		exit(0);
	}

	Platform::debugPrint("  Completed. \n");
}

ShaderProgram::ShaderProgram(std::string vertexShaderFilePath, std::string geometryShaderFilePath, std::string fragmentShaderFilePath)
{
	_programID = 0;
	_vertexShaderPath = vertexShaderFilePath;
	_geometryShaderPath = geometryShaderFilePath;
	_fragmentShaderPath = fragmentShaderFilePath;

	_hasGeometryShader = true;

	Platform::debugPrint("\nCompiling vertex shader. FilePath : %s\n", vertexShaderFilePath.c_str());
	GLint vertexShaderObj = CompileShader(_vertexShaderPath.c_str(), GL_VERTEX_SHADER);
	Platform::debugPrint("Compiling geometry shader. FilePath : %s\n", geometryShaderFilePath.c_str());
	GLint geometryShaderObj = CompileShader(geometryShaderFilePath.c_str(), GL_GEOMETRY_SHADER);
	Platform::debugPrint("Compiling fragment shader. FilePath : %s\n", fragmentShaderFilePath.c_str());
	GLint fragmentShaderObj = CompileShader(_fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);

	Platform::debugPrint("Linking program...");
	_programID = glCreateProgram();
	glAttachShader(_programID, vertexShaderObj);
	glAttachShader(_programID, geometryShaderObj);
	glAttachShader(_programID, fragmentShaderObj);
	glLinkProgram(_programID);


	GLint linkStatus = GL_FALSE;
	int infoLogLength = 0;

	glGetProgramiv(_programID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &infoLogLength);

	glDetachShader(_programID, vertexShaderObj);
	glDetachShader(_programID, geometryShaderObj);
	glDetachShader(_programID, fragmentShaderObj);

	glDeleteShader(vertexShaderObj);
	glDeleteShader(geometryShaderObj);
	glDeleteShader(fragmentShaderObj);

	if (infoLogLength > 0)
	{
		std::vector<char> programInfoLog(infoLogLength + 1);
		glGetProgramInfoLog(_programID, infoLogLength, NULL, &programInfoLog[0]);
		Platform::debugPrint("%s\n", &programInfoLog[0]);
	}

	if (!linkStatus)
	{
		throw std::exception("Linking failed...");
		exit(0);
	}
	Platform::debugPrint("  Completed.\n");
}

bool ShaderProgram::HasGeometryShader()
{
	return _hasGeometryShader;
}

std::string ShaderProgram::GetVertexShaderFilePath()
{
	return _vertexShaderPath;
}

std::string ShaderProgram::GetGeometryShaderFilePath()
{
	return _geometryShaderPath;
}

std::string ShaderProgram::GetFragmentShaderFilePath()
{
	return _fragmentShaderPath;
}

GLint ShaderProgram::CompileShader(const char* shaderFilePath, GLenum shaderType)
{
	char* shaderFileData = NULL;
	FILE* fp = fopen(shaderFilePath, "rb");

	if (fp)
	{
		unsigned int shaderFileLen = FileReader::GetLength(shaderFilePath);
		shaderFileData = (char*)malloc(shaderFileLen + 1);
		memset(shaderFileData, (int)'\0', shaderFileLen + 1);
		fread(shaderFileData, 1, shaderFileLen, fp);
		fclose(fp);
	}
	else
	{
		throw std::exception("\nError: Failed to open shader file.");
		exit(0);
	}

	GLuint shaderObject = glCreateShader(shaderType);
	glShaderSource(shaderObject, 1, (const char**)(&(shaderFileData)), NULL);
	glCompileShader(shaderObject);

	free(shaderFileData);

	GLint shaderCompileStatus = GL_FALSE;
	int compilationLogLength;

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);
	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compilationLogLength);

	std::vector<char> shaderCompileLog(compilationLogLength + 1);
	glGetShaderInfoLog(shaderObject, compilationLogLength, NULL, &shaderCompileLog[0]);

	if (compilationLogLength > 0)
		Platform::debugPrint("\n%s\n", &shaderCompileLog[0]);

	if (!shaderCompileStatus)
	{
		throw std::exception("\nError: Failed to compile shader.");
		exit(0);
	}

	return shaderObject;
}

void ShaderProgram::SetUniformMatrix4fv(const char* str, const float* mat)
{
	GLint location = glGetUniformLocation(_programID, str);
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniformMatrix3fv(const char* str, const float* mat)
{
	GLint location = glGetUniformLocation(_programID, str);
	glUniformMatrix3fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniformMatrix4fv(const char* str, glm::mat4& mat)
{
	SetUniformMatrix4fv(str, glm::value_ptr(mat));
}

void ShaderProgram::SetUniformMatrix3fv(const char* str, glm::mat3& mat)
{
	SetUniformMatrix3fv(str, glm::value_ptr(mat));
}

void ShaderProgram::SetUniform4f(const char* str, float v1, float v2, float v3, float v4)
{
	glUniform4f(glGetUniformLocation(_programID, str), v1, v2, v3, v4);
}

void ShaderProgram::SetUniform3f(const char* str, float v1, float v2, float v3)
{
	glUniform3f(glGetUniformLocation(_programID, str), v1, v2, v3);
}

void ShaderProgram::SetUniform2f(const char* str, float v1, float v2)
{
	glUniform2f(glGetUniformLocation(_programID, str), v1, v2);
}

void ShaderProgram::SetUniform1f(const char* str, const float val)
{
	glUniform1f(glGetUniformLocation(_programID, str), val);
}

void ShaderProgram::SetUniform1i(const char* str, int val)
{
	glUniform1i(glGetUniformLocation(_programID, str), val);
}

void ShaderProgram::SetUniform4f(const char* str, glm::vec4& vec)
{
	SetUniform4f(str, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetUniform3f(const char* str, glm::vec3& vec)
{
	SetUniform3f(str, vec.x, vec.y, vec.z);
}

void ShaderProgram::SetUniform2f(const char* str, glm::vec2& vec)
{
	SetUniform2f(str, vec.x, vec.y);
}

//////////////// Location based methods ////////////////


void ShaderProgram::SetUniformMatrix4fv(GLint location, const float* mat)
{
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniformMatrix3fv(GLint location, const float* mat)
{
	glUniformMatrix3fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniformMatrix4fv(GLint location, glm::mat4& mat)
{
	SetUniformMatrix4fv(location, glm::value_ptr(mat));
}

void ShaderProgram::SetUniformMatrix3fv(GLint location, glm::mat3& mat)
{
	SetUniformMatrix3fv(location, glm::value_ptr(mat));
}

void ShaderProgram::SetUniform4f(GLint location, float v1, float v2, float v3, float v4)
{
	glUniform4f(location, v1, v2, v3, v4);
}

void ShaderProgram::SetUniform3f(GLint location, float v1, float v2, float v3)
{
	glUniform3f(location, v1, v2, v3);
}

void ShaderProgram::SetUniform2f(GLint location, float v1, float v2)
{
	glUniform2f(location, v1, v2);
}

void ShaderProgram::SetUniform1f(GLint location, const float val)
{
	glUniform1f(location, val);
}

void ShaderProgram::SetUniform1i(GLint location, int val)
{
	glUniform1i(location, val);
}

void ShaderProgram::SetUniform4f(GLint location, glm::vec4& vec)
{
	SetUniform4f(location, vec.x, vec.y, vec.z, vec.w);
}

void ShaderProgram::SetUniform3f(GLint location, glm::vec3& vec)
{
	SetUniform3f(location, vec.x, vec.y, vec.z);
}

void ShaderProgram::SetUniform2f(GLint location, glm::vec2& vec)
{
	SetUniform2f(location, vec.x, vec.y);
}

//////////////////////////////////////////////////////////////////

GLint ShaderProgram::GetUniformLocation(const char* str)
{
	return glGetUniformLocation(_programID, str);
}

GLint ShaderProgram::GetAttribLocation(const char* str)
{
	return glGetAttribLocation(_programID, str);
}

GLuint ShaderProgram::ProgramID()
{
	return _programID;
}

void ShaderProgram::Begin()
{
	glUseProgram(_programID);
}

void ShaderProgram::End()
{
	glUseProgram(0);
}

ShaderProgram::~ShaderProgram()
{
	if (_programID)
		glDeleteProgram(_programID);
}
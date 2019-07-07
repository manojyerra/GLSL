#include "ShaderProgram.h"
#include "CFileReader.h"


ShaderProgram::ShaderProgram(string vertexShaderFilePath, string fragmentShaderFilePath)
{
	_programID = 0;
	_vertexShaderPath = vertexShaderFilePath;
	_geometryShaderPath = "";
	_fragmentShaderPath = fragmentShaderFilePath;

	printf("Compiling vertex shader. FilePath : %s\n", vertexShaderFilePath.c_str());
	GLint vertexShaderObj = CompileShader(_vertexShaderPath.c_str(), GL_VERTEX_SHADER);
	printf("Compiling fragment shader. FilePath : %s\n", fragmentShaderFilePath.c_str());
	GLint fragmentShaderObj = CompileShader(_fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);

	printf("Linking program...\n");
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
		printf("%s\n", &programInfoLog[0]);
	}

	if(!linkStatus)
	{
		throw std::exception("Linking failed...");
		exit(0);
	}
	printf("\n");
}

ShaderProgram::ShaderProgram(string vertexShaderFilePath, string geometryShaderFilePath, string fragmentShaderFilePath)
{
	_programID = 0;
	_vertexShaderPath = vertexShaderFilePath;
	_geometryShaderPath = geometryShaderFilePath;
	_fragmentShaderPath = fragmentShaderFilePath;

	printf("Compiling vertex shader. FilePath : %s\n", vertexShaderFilePath.c_str());
	GLint vertexShaderObj = CompileShader(_vertexShaderPath.c_str(), GL_VERTEX_SHADER);
	printf("Compiling geometry shader. FilePath : %s\n", geometryShaderFilePath.c_str());
	GLint geometryShaderObj = CompileShader(geometryShaderFilePath.c_str(), GL_GEOMETRY_SHADER_EXT);
	printf("Compiling fragment shader. FilePath : %s\n", fragmentShaderFilePath.c_str());
	GLint fragmentShaderObj = CompileShader(_fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER);

	printf("Linking program...\n");
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
		printf("%s\n", &programInfoLog[0]);
	}

	if (!linkStatus)
	{
		throw std::exception("Linking failed...");
		exit(0);
	}
	printf("\n");
}

string ShaderProgram::GetVertexShaderFilePath()
{
	return _vertexShaderPath;
}

string ShaderProgram::GetGeometryShaderFilePath()
{
	return _geometryShaderPath;
}

string ShaderProgram::GetFragmentShaderFilePath()
{
	return _fragmentShaderPath;
}

GLint ShaderProgram::CompileShader(const char* shaderFilePath, GLenum shaderType)
{
	char* shaderFileData = NULL;
	FILE* fp = fopen(shaderFilePath, "rb");

    if(fp)
    {
		unsigned int shaderFileLen = CFileReader::GetLength(shaderFilePath);
		shaderFileData = (char*)malloc(shaderFileLen+1);
		memset(shaderFileData, (int)'\0', shaderFileLen+1);
		fread(shaderFileData, 1, shaderFileLen, fp);
		fclose(fp);
	}
	else
	{
		throw std::exception("\nError: Failed to open shader file.");
		exit(0);
	}

	GLuint shaderObject =  glCreateShader(shaderType);
	glShaderSource(shaderObject, 1, (const char**)(&(shaderFileData)), NULL);
	glCompileShader(shaderObject);

	free(shaderFileData);

	GLint shaderCompileStatus = GL_FALSE;
	int compilationLogLength;

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);
	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compilationLogLength);

	std::vector<char> shaderCompileLog(compilationLogLength+1);
	glGetShaderInfoLog(shaderObject, compilationLogLength, NULL, &shaderCompileLog[0]);

	if (compilationLogLength > 0)
		printf("\n%s\n", &shaderCompileLog[0]);

	if(!shaderCompileStatus)
	{
		throw std::exception("\nError: Failed to compile shader.");
		exit(0);
	}

	return shaderObject;
}

void ShaderProgram::SetUniformMatrix4fv(const char* str, float* mat)
{
	GLint location = glGetUniformLocation(_programID, str);
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniformMatrix3fv(const char* str, float* mat)
{
	GLint location = glGetUniformLocation(_programID, str);
	glUniformMatrix3fv(location, 1, GL_FALSE, mat);
}

void ShaderProgram::SetUniform1f(const char* str, float val)
{
	glUniform1f(glGetUniformLocation(_programID, str), val);
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
	if(_programID)
		glDeleteProgram(_programID);
}

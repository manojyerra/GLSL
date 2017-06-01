#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#define glCreateShader glCreateShaderObjectARB
#define GL_VERTEX_SHADER GL_VERTEX_SHADER_ARB
#define GL_FRAGMENT_SHADER GL_FRAGMENT_SHADER_ARB

int CreateGlutWindow(char* title, int x, int y, int w, int h);
void Display();

GLint CreateShaderProgram(char* vertexShaderFilePath, char* fragmentShaderFilePath);
GLint CompileShader(char* shaderFilePath, GLenum shaderType);
unsigned int GetFileLength(string filePath);


GLuint vertexbuffer;
GLuint programID;

static const GLfloat g_vertex_buffer_data[] =
{
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,500,500);
	glewInit();

	programID =  CreateShaderProgram("shaders/vertexshader.txt","shaders/fragmentshader.txt");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}


int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);
	return glutCreateWindow(title);
}


void Display()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	glUseProgram(0);

	glutSwapBuffers();
}


GLint CreateShaderProgram(char* vertexShaderFilePath, char* fragmentShaderFilePath)
{
	printf("\nCompiling vertex shader...\n");
	GLint vertexShaderObj = CompileShader(vertexShaderFilePath, GL_VERTEX_SHADER);

	if(!vertexShaderObj)
		return -1;

	printf("\nCompiling fragment shader...\n");
	GLint fragmentShaderObj = CompileShader(fragmentShaderFilePath, GL_FRAGMENT_SHADER);

	if(!fragmentShaderObj)
		return -1;

	// Link the program
	printf("\nLinking program\n");
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderObj);
	glAttachShader(programID, fragmentShaderObj);
	glLinkProgram(programID);

	// Check the program
	GLint linkStatus = GL_FALSE;
	int infoLogLength;

	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	glDetachShader(programID, vertexShaderObj);
	glDetachShader(programID, fragmentShaderObj);
	
	glDeleteShader(vertexShaderObj);
	glDeleteShader(fragmentShaderObj);

	if(!linkStatus)
	{
		printf("\nLinking failed...");

		if(infoLogLength > 0)
		{
			std::vector<char> programInfoLog(infoLogLength+1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programInfoLog[0]);
			printf("%s\n", &programInfoLog[0]);
		}

		return -1;
	}
	else
	{
		if(infoLogLength > 0)
		{
			std::vector<char> programInfoLog(infoLogLength+1);
			glGetProgramInfoLog(programID, infoLogLength, NULL, &programInfoLog[0]);
			printf("%s\n", &programInfoLog[0]);
		}
	}

	return programID;
}


unsigned int GetFileLength(string filePath)
{
	FILE  *fp = fopen(filePath.c_str(), "rb");
    
    if(fp)
    {
        fseek(fp, 0, SEEK_END);
        unsigned int fileSize = (unsigned int)ftell(fp);
        fseek(fp, 0, SEEK_SET);
        fclose(fp);
        return fileSize;
    }
    
    return 0;
}


GLint CompileShader(char* shaderFilePath, GLenum shaderType)
{
	unsigned int shaderFileLen = GetFileLength( shaderFilePath );
	
	if(shaderFileLen == 0)
	{
		printf("\nError : Shader file length is zero.\n");
		return -1;
	}

	FILE* fp = fopen(shaderFilePath, "rb");
	char* shaderFileData = NULL;

    if(fp)
    {
		shaderFileData = (char*)malloc(shaderFileLen+1);
		memset(shaderFileData, (int)'\0', shaderFileLen+1);
		fread(shaderFileData, 1, shaderFileLen, fp);
		fclose(fp);
	}
	else
	{
		printf("\nError : Unable to open shader file.\n");
		return -1;	//unable to open file.
	}

	GLuint shaderObject =  glCreateShader(shaderType);
	glShaderSourceARB(shaderObject, 1, (const char**)(&(shaderFileData)), NULL);
	glCompileShader(shaderObject);

	free(shaderFileData);

	GLint shaderCompileStatus = GL_FALSE;
	int compilationLogLength;

	glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &shaderCompileStatus);
	glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &compilationLogLength);

	std::vector<char> shaderCompileLog(compilationLogLength+1);
	glGetShaderInfoLog(shaderObject, compilationLogLength, NULL, &shaderCompileLog[0]);

	if(!shaderCompileStatus)
	{
		printf("\nCompilation Failed");
			
		if(compilationLogLength > 0)
			printf("\n%s\n", &shaderCompileLog[0]);

		return -1;
	}
	else
	{
		if(compilationLogLength > 0)
			printf("\n%s\n", &shaderCompileLog[0]);
	}

	return shaderObject;
}


/*
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

#define glCreateShader glCreateShaderObjectARB
#define GL_VERTEX_SHADER GL_VERTEX_SHADER_ARB
#define GL_FRAGMENT_SHADER GL_FRAGMENT_SHADER_ARB

int CreateGlutWindow(char* title, int x, int y, int w, int h);
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
void Display();

GLuint vertexbuffer;
GLuint programID;

static const GLfloat g_vertex_buffer_data[] =
{
   -1.0f, -1.0f, 0.0f,
   1.0f, -1.0f, 0.0f,
   0.0f,  1.0f, 0.0f,
};


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	CreateGlutWindow("First Window", 0,0,500,500);
	glewInit();

	programID =  LoadShaders("shaders/vertexshader.txt","shaders/fragmentshader.txt");

	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glutDisplayFunc(Display);
	glutMainLoop();

	return 0;
}


int CreateGlutWindow(char* title, int x, int y, int w, int h)
{
	glutInitWindowPosition(x,y);
	glutInitWindowSize(w,h);
	return glutCreateWindow(title);
}


void Display()
{
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
	glDisableVertexAttribArray(0);

	glUseProgram(0);

	glutSwapBuffers();
}


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path)
{
	// Create the shaders
	GLuint VertexShaderID =  glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID =  glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::string Line = "";
		while(getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::string Line = "";
		while(getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}


	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}
	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}
*/
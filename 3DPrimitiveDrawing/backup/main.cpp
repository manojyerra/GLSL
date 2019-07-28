#include "GLInclude.h"
#include "GameLoop.h"
#include "Input.h"
#include "vld.h"

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GameLoop* gameLoop = NULL;

int main(void)
{
	long startTime = GetTickCount();
	Platform::debugPrint("\n\nBegin: Loading resources\n\n");

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DEPTH_BITS, 24);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	//TODO: How to change number of samples runtime and disable/enable sampling runtime ( mainly for while drawing an object ).
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	int sw = 1280;//(int)(mode->width*0.85f);
	int sh = 720; //(int)(mode->height*0.85f);
	int sx = (int)((mode->width - sw) / 2.0f);
	int sy = (int)((mode->height - sh) / 2.0f);

	GLFWwindow* window = glfwCreateWindow(sw, sh, "GLFW Window", NULL, NULL);
	glfwSetWindowPos(window, sx, sy);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//if (glewInit() != GLEW_OK)
	//{
	//	throw new exception("\nError: Failed to initialize glew.\n");
	//}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		throw new exception("\nError: Failed to initialize OpenGL context.\n");
	}

	glfwSwapInterval(1);

	gameLoop = new GameLoop((float)sw, (float)sh);
	Input::Init();

	double previousTime = glfwGetTime();
	unsigned int frameCount = 0;

	Platform::debugPrint("\nEnd: Loading resources, Load Time : %ld milliseconds\n\n", GetTickCount() - startTime);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		for (int i = 0; i <Input::NUM_KEYS; i++)
			Input::storeKeyStates[i] = glfwGetKey(window, i);

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		Input::Update((int)xpos, (int)ypos, 1.0f / 30.0f);
		gameLoop->Update(1.0f / 30.0f);
		gameLoop->Draw();

		glfwSwapBuffers(window);

		frameCount++;
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;

		if(deltaTime > 0.5)
		{
			frameCount = (unsigned int)(frameCount*1.0 / deltaTime);
			char arr[128];
			sprintf(arr, "FPS : %d", frameCount);
			glfwSetWindowTitle(window, arr);

			frameCount = 0;
			previousTime = currentTime;
		}
	}

	if (gameLoop)
	{
		delete gameLoop;
		gameLoop = NULL;
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	Platform::debugPrint("\nOnSizeChange : sw = %d sh = %d", width, height);

	if (gameLoop && width > 10 && height > 10)
	{
		gameLoop->SetScreenSize((float)width, (float)height);
	}
	else
	{
		Platform::debugPrint("\n\n ***** Ignoring framebuffer size callback. **** \n\n");
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			Input::LEFT_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::LEFT_BUTTON_DOWN = false;
		}
	}
	else if(button == GLFW_MOUSE_BUTTON_MIDDLE)
	{
		if (action == GLFW_PRESS)
		{
			Input::MIDDLE_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::MIDDLE_BUTTON_DOWN = false;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
		{
			Input::RIGHT_BUTTON_DOWN = true;
		}
		else if (action == GLFW_RELEASE)
		{
			Input::RIGHT_BUTTON_DOWN = false;
		}
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Input::SCROLL_STATE_STORE = (yoffset < 0) ? Input::SCROLL_DOWN : Input::SCROLL_UP;
}




/*

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/Camera.h>

#include "ShaderProgram.h"
#include "ShadersManager.h"

#include <iostream>
#include <random>
#include "ObjReader.h"
#include "GLMeshRenderer.h"
#include "GLSSAO.h"
#include "Cam.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path, bool gammaCorrection);
void renderQuad();
//void renderCube();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lerp(float a, float b, float f)
{
	return a + f * (b - a);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 16);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glEnable(GL_DEPTH_TEST);

	ShaderProgram shaderGeometryPass("shaders/ssao/ssao_geometry.vs", "shaders/ssao/ssao_geometry.fs");
	ShaderProgram shaderSSAO("shaders/ssao/ssao.vs", "shaders/ssao/ssao.fs");

	GLMeshRenderer nanosuit(&ObjReader("data/demo/nanosuite"), GLMeshRenderer::PHONG_PER_PIXEL_SHADER);
	GLSSAO ssao(SCR_WIDTH, SCR_HEIGHT);

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glDisable(GL_CULL_FACE);

	float _zNear = 0.15f;
	float _zFar = 1000000.0f;
	float _zNearPlaneW = 0.2f;

	Cam::GetInstance()->Init(SCR_WIDTH, SCR_HEIGHT, _zNear, _zFar, _zNearPlaneW);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// 1. geometry pass: render scene's geometry/color data into gbuffer
		// -----------------------------------------------------------------
		glBindFramebuffer(GL_FRAMEBUFFER, ssao.GetGBufferFBO());
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Cam* cam = Cam::GetInstance();
		GLMat modelMat;

		shaderGeometryPass.Begin();
		shaderGeometryPass.SetUniformMatrix4fv("projection", cam->GetProjMat());
		shaderGeometryPass.SetUniformMatrix4fv("view", cam->GetViewMat());
		shaderGeometryPass.SetUniformMatrix4fv("model", modelMat.m);

		glBindVertexArray(vao);

		if (nanosuit._bufferBuilder->GetNormalBufferID())
		{
			GLuint normalLoc = glGetAttribLocation(shaderGeometryPass.ProgramID(), "aNormal");
			glEnableVertexAttribArray(normalLoc);
			glBindBuffer(GL_ARRAY_BUFFER, nanosuit._bufferBuilder->GetNormalBufferID());
			glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_TRUE, 0, (void*)0);
		}

		GLuint vertexLoc = glGetAttribLocation(shaderGeometryPass.ProgramID(), "aPos");
		glEnableVertexAttribArray(vertexLoc);
		glBindBuffer(GL_ARRAY_BUFFER, nanosuit._bufferBuilder->GetVertexBufferID());
		glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glDrawArrays(GLMeshRenderer::triangles, 0, (GLsizei)(nanosuit._bufferBuilder->GetVertexBufferSize() / 12));
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


		// 2. generate SSAO texture
		glClear(GL_COLOR_BUFFER_BIT);
		shaderSSAO.Begin();
		shaderSSAO.SetUniform1i("gPosition", 0);
		shaderSSAO.SetUniform1i("gNormal", 1);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ssao.GetGPositionTexID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, ssao.GetGNormalTexID());
		renderQuad();


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
*/




/*
glfwWindowHint(GLFW_RED_BITS, mode->redBits);
glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

glfwMaximizeWindow(window);

std::cout << glGetString(GL_VERSION) << std::endl;
*/

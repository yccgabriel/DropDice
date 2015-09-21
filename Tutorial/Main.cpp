#ifdef _DEBUG
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
	#include <windows.h>
	#include <wincon.h>		// for debugging console
	#define _CRTDBG_MAP_ALLOC	// debug memory leak
	#include <stdlib.h>
	#include <crtdbg.h>
//	#ifndef DBG_NEW
//	#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )     
//	#define new DBG_NEW
//	#endif
#endif
#endif

#define GLEW_STATIC
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600
#include <thread>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <qu3e/q3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtil.h"
#include "camera.h"

#include "ShaderManager.h"
#include "SpawnMachine.h"
#include "Floor.h"

Camera camera;

void glfw_error_callback(int error, const char* description)
{
#ifdef _DEBUG
	puts(description);
#endif
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	else if (key == GLFW_KEY_W && action == GLFW_PRESS)
		camera.Move(FORWARD);
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
		camera.Move(LEFT);
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
		camera.Move(BACK);
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
		camera.Move(RIGHT);
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		camera.Move(DOWN);
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
		camera.Move(UP);
}
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		camera.move_camera = true;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		camera.move_camera = false;
}
void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	camera.Move2D(x, y);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		camera.camera_position_delta += camera.camera_up * .05f;
	else if (yoffset < 0)
		camera.camera_position_delta -= camera.camera_up * .05f;
}

q3Scene scene(1.0 / 60.0);

int main()
{
#ifdef _DEBUG
	std::cout << "Debugging Window:\n" << std::endl;
//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
//	_CrtSetBreakAlloc(527);
#endif

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
	{
#ifdef _DEBUG
		std::cout << "glfwInit() failed" << std::endl;
#endif
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);						// OpenGL3.2
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// specify only support new core functionality of OpenGL3.2
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL", nullptr, nullptr);	// width, height, window title, null for window mode|glfwGetPrimaryMonitor() for full screen, OpenGL context to share resource
	if (window == NULL)
	{
#ifdef _DEBUG
		std::cout << "glfwCreateWindow() cannot create window. Check hardware support" << std::endl;
#endif
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);
	
	glewExperimental = GL_TRUE;
	GLenum err=glewInit();
	if (err != GLEW_OK)
	{
#ifdef _DEBUG
		std::cout << "glewInit() failed, aborting. Code " << err << ". Error: " << glewGetErrorString(err) << std::endl;
#endif
	}

	// Vertex Array Objects, use to link VBO and attributes with raw vertex data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	ShaderManager *shaderManager = ShaderManager::GetInstance();

	SpawnMachine spawnMachine(SpawnMachine::DROPDICE, 1000);
	Floor* floor = new Floor();;

	shaderManager->ActivateProgram();

	glEnable(GL_DEPTH_TEST);
	GLint overrideColor = glGetUniformLocation(shaderManager->resources.shaderProgram, "overrideColor");
	glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);

	// Camera Section
	glm::mat4 model, view, proj;

	camera.SetPosition(glm::vec3(2.0f, 2.0f, 5.0f));
	camera.SetLookAt(glm::vec3(0, 0, 0));
	camera.SetClipping(0.1, 1000);
	camera.SetFOV(45);
	camera.SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	floor->CreateInstance();

	spawnMachine.Start();
	while (!glfwWindowShouldClose(window))
	{
		scene.Step();

		//glEnable(GL_DEPTH_TEST);		// this will fill the depth buffer with zeros, black screen

		// Clear the screen to black
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		spawnMachine.DrawAllDice();

	//	glEnable(GL_STENCIL_TEST);
	//		// Draw floor
	//		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//		glStencilMask(0xFF); // Write to stencil buffer
	//		glDepthMask(GL_FALSE); // Don't write to depth buffer
	//		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			Instance* floor0 = floor->mInstances[0];
			floor->DrawInstance(floor0);
	//	
	//		// Draw cube reflection
	//		glStencilFunc(GL_EQUAL, 1, 0xFF);
	//		glStencilMask(0x00); // Write nothing to stencil buffer
	//		glDepthMask(GL_TRUE); // why? write to depth buffer
	//		model = glm::translate(model, glm::vec3(0, 0, -1));
	//		model = glm::scale(model, glm::vec3(1, 1, -1));
	//		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	//	
	//		glUniform3f(overrideColor, 0.3f, 0.3f, 0.3f);
	//		//die.Draw();
	//		glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);
	//
	//
	//	glDisable(GL_STENCIL_TEST);

		camera.Update();
		camera.GetMatricies(proj, view, model);
		glUniformMatrix4fv(shaderManager->resources.uniforms.model , 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shaderManager->resources.uniforms.proj  , 1, GL_FALSE, glm::value_ptr(proj));
		glUniformMatrix4fv(shaderManager->resources.uniforms.view  , 1, GL_FALSE, glm::value_ptr(view));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	spawnMachine.Stop();

	delete shaderManager;

	floor->DeleteAllInstances();
	delete floor;

	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	getchar();
	return 0;
}
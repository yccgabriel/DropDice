#ifdef _DEBUG
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
#include <windows.h>
#include <wincon.h>		// for debugging console
#endif
#endif

#define GLEW_STATIC
#include <thread>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <bullet/btBulletDynamicsCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtil.h"

#include "ShaderManager.h"
#include "Floor.h"
#include "Die.h"

btBroadphaseInterface* broadphase;
btDefaultCollisionConfiguration* collisionConfiguration;
btCollisionDispatcher* dispatcher;
btSequentialImpulseConstraintSolver* solver;
btDiscreteDynamicsWorld* dynamicsWorld;

void glfw_error_callback(int error, const char* description)
{
#ifdef _DEBUG
	puts(description);
#endif
}

int main()
{
#ifdef _DEBUG
	//AllocConsole();
	std::cout << "Debugging Window:\n" << std::endl;
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

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);	// width, height, window title, null for window mode|glfwGetPrimaryMonitor() for full screen, OpenGL context to share resource
	//GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr);
	if (window == NULL)
	{
#ifdef _DEBUG
		std::cout << "glfwCreateWindow() cannot create window. Check hardware support" << std::endl;
#endif
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	GLenum err=glewInit();
	if (err != GLEW_OK)
	{
#ifdef _DEBUG
		std::cout << "glewInit() failed, aborting. Code " << err << ". Error: " << glewGetErrorString(err) << std::endl;
#endif
	}

	//** Bullet Physics Engine
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, 0, -10));

	// Vertex Array Objects, use to link VBO and attributes with raw vertex data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	ShaderManager *shaderManager = ShaderManager::GetInstance();

	Die		die;
	Floor	floor;

	shaderManager->ActivateProgram();

	// Setup MVP outside the loop. Model need to calc everytime inside the loop
	GLint uniModel = glGetUniformLocation(shaderManager->resources.shaderProgram, "model");
	GLint uniView = glGetUniformLocation(shaderManager->resources.shaderProgram, "view");
	GLint uniProj = glGetUniformLocation(shaderManager->resources.shaderProgram, "proj");
	// View transformation
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	// Projection transform
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);	//FOV, aspect ratio of screen, near, far clipping plane
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// Blending set timer
	GLint blendRatio = glGetUniformLocation(shaderManager->resources.shaderProgram, "blendRatio");
	glUniform1f(blendRatio, 0.0f);
	auto t_start = std::chrono::high_resolution_clock::now();

	glEnable(GL_DEPTH_TEST);
	GLint overrideColor = glGetUniformLocation(shaderManager->resources.shaderProgram, "overrideColor");
	glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);

	die.CreateInstance();
	//die.m_opvInstances[0]->transform = glm::mat4
	floor.CreateInstance();

	while (!glfwWindowShouldClose(window))
	{
		dynamicsWorld->stepSimulation(1 / 60.f, 10);

		//glEnable(GL_DEPTH_TEST);		// this will fill the depth buffer with zeros, black screen

		// Blending
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform1f(blendRatio, sin(time*4.0f) / 2.0f + 0.5f);

		// keep rotate with time
		glm::mat4 model;
	//	model = glm::rotate(
	//		model,
	//		time * glm::radians(180.0f) ,
	//		glm::vec3(0.0f, 0.0f, 1.0f)
	//	);
	//	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));	// have to update to the handler everytime

		// Clear the screen to black
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw cube
		model = glm::translate(model, glm::vec3(0, 0, 0));
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	//	for (int i = 0; i < die.m_opvInstances.size(); i++)
	//	{
	//		die.DrawInstance(die.m_opvInstances[i]);
	//		//std::cout << die.m_opvInstances[i] << std::endl;
	//	}
		Instance* die0 = die.m_opvInstances[0];
		//die0->transform = glm::translate(glm::mat4(), glm::vec3(0, 0, 1));
		die0->SetTransform();
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(die0->transform));
		die.DrawDie();


	//	glEnable(GL_STENCIL_TEST);
	//		// Draw floor
	//		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//		glStencilMask(0xFF); // Write to stencil buffer
	//		glDepthMask(GL_FALSE); // Don't write to depth buffer
	//		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			Instance* floor0 = floor.m_opvInstances[0];
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(floor0->transform));
			floor.DrawFloor();
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

		// Triangle color keep varying
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		//glUniform3f(uniColor, (sin(time*4.0f) + 1.0f / 2.0f), (sin(time*2.0f) + 1.0f / 2.0f), (sin(time*1.0f) + 1.0f / 2.0f));


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete shaderManager;

	// Clean up Bullet Physics Engine
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	glDeleteVertexArrays(1, &vao);


	glfwTerminate();
	return 0;
}
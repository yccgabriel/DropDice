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

#include <imgui/imgui.h>
#include "imgui_impl_glfw_gl3.h"

#include "ShaderUtil.h"
#include "camera.h"

#include "ShaderManager.h"
#include "Scene.h"
#include "SpawnMachine.h"

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
bool cursor_in_background(GLFWwindow* window, double x, double y)
{
	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);

	GLfloat depth;	// farthest = 1, nearest = 0;
	glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	return depth == 1 ? true: false;
}
glm::vec3 ray_dir(GLFWwindow* window, double xx, double yy)
{
	int window_width, window_height;
	glfwGetFramebufferSize(window, &window_width, &window_height);

	float x = (2.0f * xx) / window_width - 1.0f;
	float y = 1.0f - (2.0f * yy) / window_height;
	float z = 1.0f;
	glm::vec3 ray_nds = glm::vec3(x, y, z);		// normalized device coordinates

	glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
	
	glm::vec4 ray_eye = glm::inverse(camera.GetProjectionMatrix()) * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

	glm::vec4 ivxr = glm::inverse(camera.GetViewMatrix())*ray_eye;
	glm::vec3 ray_wor = glm::vec3(ivxr.x, ivxr.y, ivxr.z);
	ray_wor = glm::normalize(ray_wor);

	return ray_wor;
}
SceneMachine* pSceneMachine;		//	this variable points to a stack!!!
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS 
		&& cursor_in_background(window, x, y) == true
		&& ImGui::IsWindowHovered() == true		)		// clicked in background
		camera.move_camera = true;
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS
		&& cursor_in_background(window, x, y) == false
		&& ImGui::IsWindowHovered() == true)		// clicked in whatever object
	{
		glm::vec3 ray_direction = ray_dir(window, x, y);						// in world space
		Instance* instance = pSceneMachine->PickNearestInstance(camera.camera_position, ray_direction);		// ray_origin and ray_direction
		int type;
		if ((type = pSceneMachine->ClassifyInstance(instance)) == SceneMachine::FLOOR)
			pSceneMachine->ClickOnFloor(instance);
		else if (type == SceneMachine::ROD || type == SceneMachine::CUBE)
			pSceneMachine->ClickOnCore(instance, type, RayTracer::Line(camera.camera_position, ray_direction));
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		camera.move_camera = false;
		pSceneMachine->ReleaseCore();
	}
}
void cursor_pos_callback(GLFWwindow* window, double x, double y)
{
	camera.Move2D(x, y);
	if (pSceneMachine->mRubiksCore.mTranslateCore == true)
		pSceneMachine->mRubiksCore.TranslateCore(camera.camera_position, ray_dir(window, x, y));
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (yoffset > 0)
		camera.camera_position_delta += camera.camera_up * .05f;
	else if (yoffset < 0)
		camera.camera_position_delta -= camera.camera_up * .05f;
}

q3Scene q3scene(1.0 / 60.0);

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
	
	glewExperimental = GL_TRUE;
	GLenum err=glewInit();
	if (err != GLEW_OK)
	{
#ifdef _DEBUG
		std::cout << "glewInit() failed, aborting. Code " << err << ". Error: " << glewGetErrorString(err) << std::endl;
#endif
	}

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);
	ImVec4 clear_color = ImColor(114, 144, 154);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Vertex Array Objects, use to link VBO and attributes with raw vertex data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	ShaderManager *shaderManager = ShaderManager::GetInstance();

	q3scene.SetGravity(q3Vec3(0,0,-10));

	SpawnMachine spawnMachine(SpawnMachine::DROPDICE, 1000);
	SceneMachine sceneMachine;
	pSceneMachine = &sceneMachine;

	shaderManager->ActivateProgram();

	glEnable(GL_DEPTH_TEST);
	GLint overrideColor = glGetUniformLocation(shaderManager->resources.shaderProgram, "overrideColor");
	glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);

	// Camera Section
	glm::mat4 model, view, proj;

	camera.SetPosition(glm::vec3(1.0f, 1.0f, 5.0f));
	camera.SetLookAt(glm::vec3(0, 0, 0));
	camera.SetClipping(0.1, 1000);
	camera.SetFOV(45);
	camera.SetViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	sceneMachine.SetScene();

	spawnMachine.Start();
	while (!glfwWindowShouldClose(window))
	{
		q3scene.Step();

		//glEnable(GL_DEPTH_TEST);		// this will fill the depth buffer with zeros, black screen

		// Clear the screen to black
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		spawnMachine.CheckSpawn();
		spawnMachine.DrawAllDice();

	//	glEnable(GL_STENCIL_TEST);
	//		// Draw floor
	//		glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
	//		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//		glStencilMask(0xFF); // Write to stencil buffer
	//		glDepthMask(GL_FALSE); // Don't write to depth buffer
	//		glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			sceneMachine.Render();
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


		// ImGui
		ImGui_ImplGlfwGL3_NewFrame();

		static float f = 0.0f;
		ImGui::Text("Hello, World!");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		ImGui::ColorEdit3("clear color", (float*)&clear_color);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		// rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		ImGui::Render();
		// End of ImGui


		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	spawnMachine.Stop();

	delete shaderManager;

	glDeleteVertexArrays(1, &vao);

	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
	//getchar();
#endif
	return 0;
}
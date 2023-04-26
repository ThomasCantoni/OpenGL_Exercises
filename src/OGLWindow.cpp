#include "OGLWindow.h"
#include "Common.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

OGLWindow::OGLWindow(int InWidth, int InHeight, const std::string& InTitle) 
    : Width(InWidth), Height(InHeight), Title(InTitle)
{
    DeltaTime = 0;
    LastTime = 0;

    DIE_ON_ERROR(glfwInit(), "GLFW Init failed!");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    /*
    bool fullScreen = false;
	GLFWmonitor* monitor = fullScreen ? glfwGetPrimaryMonitor() : NULL;
	GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), monitor, NULL);
    */
    RawWindow = glfwCreateWindow(Width, Height, Title.c_str(), NULL, NULL);
    DIE_ON_NULL(RawWindow, "Window failed!");
    
	glfwMakeContextCurrent(RawWindow);

	DIE_ON_ERROR(gladLoadGL(), "Glad Load failed!");
}

OGLWindow::~OGLWindow() 
{
    glfwDestroyWindow(RawWindow);    
}

void OGLWindow::PrintInfo() 
{
	std::cout << "GL VERSION: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GL RENDERER: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "GL VENDOR: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "GLSL VERSION: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

bool OGLWindow::IsOpened() 
{
    return !glfwWindowShouldClose(RawWindow);
}

float OGLWindow::GetDeltaTime() 
{
    return DeltaTime;
}

void OGLWindow::Update() 
{
    float currTime = glfwGetTime();
	DeltaTime = (currTime - LastTime);
	LastTime = currTime;

    glfwSwapBuffers(RawWindow);
	glfwPollEvents();
}

void OGLWindow::SetTitle(const std::string& InTitle)  {
    Title = InTitle;
    glfwSetWindowTitle(RawWindow, Title.c_str());
}